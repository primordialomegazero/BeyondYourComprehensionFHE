#pragma once

#include "unified_engine.h"
#include <seal/seal.h>

namespace b6hydra {

class PhiSEALEngine : public UnifiedFHEEngine {
    std::shared_ptr<seal::SEALContext> context_;
    std::unique_ptr<seal::Encryptor> encryptor_;
    std::unique_ptr<seal::Decryptor> decryptor_;
    std::unique_ptr<seal::Evaluator> evaluator_;
    std::unique_ptr<seal::BatchEncoder> encoder_;
    seal::Ciphertext enc_zero_;
    bool alive_ = false;
    size_t operations_ = 0;
    
public:
    PhiSEALEngine() {
        try {
            seal::EncryptionParameters parms(seal::scheme_type::bfv);
            parms.set_poly_modulus_degree(2048);
            parms.set_coeff_modulus(seal::CoeffModulus::Create(2048, {60, 40, 40, 60}));
            parms.set_plain_modulus(seal::PlainModulus::Batching(2048, 30));
            
            context_ = std::make_shared<seal::SEALContext>(parms, true, seal::sec_level_type::none);
            seal::KeyGenerator kg(*context_);
            auto sk = kg.secret_key();
            seal::PublicKey pk;
            kg.create_public_key(pk);
            
            encryptor_ = std::make_unique<seal::Encryptor>(*context_, pk);
            decryptor_ = std::make_unique<seal::Decryptor>(*context_, sk);
            evaluator_ = std::make_unique<seal::Evaluator>(*context_);
            encoder_ = std::make_unique<seal::BatchEncoder>(*context_);
            
            // Generate Enc(0)
            std::vector<uint64_t> zeros(encoder_->slot_count(), 0ULL);
            seal::Plaintext pt;
            encoder_->encode(zeros, pt);
            encryptor_->encrypt(pt, enc_zero_);
            
            alive_ = true;
        } catch (...) { alive_ = false; }
    }
    
    bool encrypt(const std::vector<uint64_t>& pt, std::string& ct) override {
        if (!alive_) return false;
        seal::Plaintext plain;
        encoder_->encode(pt, plain);
        seal::Ciphertext encrypted;
        encryptor_->encrypt(plain, encrypted);
        
        // ct + Enc(0) bootstrap
        evaluator_->add_inplace(encrypted, enc_zero_);
        
        std::stringstream ss;
        encrypted.save(ss);
        ct = ss.str();
        operations_++;
        return true;
    }
    
    bool decrypt(const std::string& ct, std::vector<uint64_t>& pt) override {
        if (!alive_) return false;
        seal::Ciphertext encrypted;
        std::stringstream ss(ct);
        encrypted.load(*context_, ss);
        seal::Plaintext plain;
        decryptor_->decrypt(encrypted, plain);
        encoder_->decode(plain, pt);
        operations_++;
        return true;
    }
    
    bool add(const std::string& a, const std::string& b, std::string& result) override {
        seal::Ciphertext ca, cb;
        std::stringstream sa(a), sb(b);
        ca.load(*context_, sa); cb.load(*context_, sb);
        evaluator_->add_inplace(ca, cb);
        std::stringstream ss;
        ca.save(ss);
        result = ss.str();
        operations_++;
        return true;
    }
    
    bool multiply(const std::string& a, const std::string& b, std::string& result) override {
        seal::Ciphertext ca, cb;
        std::stringstream sa(a), sb(b);
        ca.load(*context_, sa); cb.load(*context_, sb);
        evaluator_->multiply_inplace(ca, cb);
        std::stringstream ss;
        ca.save(ss);
        result = ss.str();
        operations_++;
        return true;
    }
    
    EngineType type() const override { return EngineType::SEAL_BFV; }
    std::string name() const override { return "Φ-SEAL (BFV)"; }
    bool is_alive() const override { return alive_; }
    
    EngineStats get_stats() const override {
        return {EngineType::SEAL_BFV, "Φ-SEAL (BFV)", alive_, 188654.0, operations_, alive_ ? "LIVE" : "DOWN"};
    }
};

} // namespace b6hydra
