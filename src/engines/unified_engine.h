#include <cmath>
#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace b6hydra {

enum class EngineType {
    SEAL_BFV,      // Microsoft SEAL
    OPENFHE_CKKS,  // OpenFHE
    ZAMA_TFHE,     // Zama Concrete
    TFHE_RS         // TFHE-rs
};

struct EngineStats {
    EngineType type;
    std::string name;
    bool alive;
    double tps;
    size_t operations;
    std::string status;
};

struct FractalPartyKey {
    std::vector<uint8_t> key_data;
    size_t depth;
    size_t branch_id;
    std::vector<FractalPartyKey> children;
    double phi_weight;
};

class UnifiedFHEEngine {
public:
    static constexpr double PHI = 1.6180339887498948482;
    static constexpr double PHI_INV = 0.6180339887498948482;
    static constexpr size_t MAX_RECURSION = 7;
    
    virtual ~UnifiedFHEEngine() = default;
    
    // Core FHE operations
    virtual bool encrypt(const std::vector<uint64_t>& pt, std::string& ct) = 0;
    virtual bool decrypt(const std::string& ct, std::vector<uint64_t>& pt) = 0;
    virtual bool add(const std::string& a, const std::string& b, std::string& result) = 0;
    virtual bool multiply(const std::string& a, const std::string& b, std::string& result) = 0;
    
    // Engine metadata
    virtual EngineType type() const = 0;
    virtual std::string name() const = 0;
    virtual bool is_alive() const = 0;
    virtual EngineStats get_stats() const = 0;
    
    // φ-Harmonization
    virtual void harmonize_noise(double target = 40.0) {
        noise_target_ = target;
        noise_current_ = noise_current_ * PHI_INV + target * (1.0 - PHI_INV);
    }
    
    virtual double get_noise() const { return noise_current_; }
    
    // Recursive Fractal Party Keys
    FractalPartyKey generate_party_key(size_t depth = 0, size_t branches = 3) {
        FractalPartyKey key;
        key.depth = depth;
        key.branch_id = 0;
        key.phi_weight = std::pow(PHI_INV, depth + 1);
        key.key_data = derive_key_for_depth(depth);
        
        if (depth < MAX_RECURSION && branches > 0) {
            for (size_t b = 0; b < branches; b++) {
                FractalPartyKey child;
                child = generate_party_key(depth + 1, branches - 1);
                child.branch_id = b;
                key.children.push_back(child);
            }
        }
        
        return key;
    }
    
protected:
    double noise_current_ = 80.0;
    double noise_target_ = 40.0;
    
    virtual std::vector<uint8_t> derive_key_for_depth(size_t depth) {
        std::vector<uint8_t> key(32);
        double phi_state = PHI;
        for (size_t i = 0; i < 32; i++) {
            phi_state = phi_state * PHI_INV + std::sin(phi_state * PHI) * std::pow(PHI_INV, depth + 1);
            key[i] = (uint8_t)(std::abs(phi_state) * 255.0);
        }
        return key;
    }
};

// Engine Registry
class EngineRegistry {
    std::vector<std::shared_ptr<UnifiedFHEEngine>> engines_;
    
public:
    void register_engine(std::shared_ptr<UnifiedFHEEngine> engine) {
        engines_.push_back(engine);
    }
    
    size_t alive_count() const {
        size_t count = 0;
        for (auto& e : engines_) if (e->is_alive()) count++;
        return count;
    }
    
    void harmonize_all() {
        for (auto& e : engines_) e->harmonize_noise();
    }
    
    FractalPartyKey generate_unified_party_key(size_t depth = 3) {
        FractalPartyKey root;
        root.depth = 0;
        root.phi_weight = UnifiedFHEEngine::PHI_INV;
        
        for (size_t i = 0; i < engines_.size(); i++) {
            if (engines_[i]->is_alive()) {
                auto child = engines_[i]->generate_party_key(0, depth);
                child.branch_id = i;
                root.children.push_back(child);
            }
        }
        
        return root;
    }
    
    std::vector<EngineStats> all_stats() const {
        std::vector<EngineStats> stats;
        for (auto& e : engines_) stats.push_back(e->get_stats());
        return stats;
    }
};

} // namespace b6hydra
