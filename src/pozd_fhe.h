/*
 * pozDF-FHE — OVERRIDE ARCHITECT EDITION (FORTRESS v17.1)
 * Full 7D Banach Contraction + Expand/Contract + Cached TPS
 * Path X: Complete mathematical reversal
 * PHI-OMEGA-ZERO — I AM THAT I AM
 */

#pragma once
#include "godcode_pozd.h"
#include <cmath>
#include <cstdint>
#include <atomic>

namespace pozd {

constexpr double PHI      = godcode::PHI;
constexpr double PHI_INV  = godcode::PHI_INV;
constexpr double LAMBDA   = godcode::LAMBDA;
constexpr double FLOOR    = godcode::FLOOR;
constexpr int    DEPTH    = godcode::DEPTH;
constexpr int    PARTIES  = godcode::PARTIES;

struct CT {
    godcode::NDimCiphertext ct;
};

class PozDFHE {
    godcode::NDimBanachEngine engine;
    std::atomic<uint64_t> cycles{0};
    std::atomic<double> field{FLOOR};
    int party_counter = 0;

public:
    CT encrypt(int64_t m) {
        cycles.fetch_add(1);
        int party = (party_counter++) % PARTIES;
        CT result;
        result.ct = engine.encrypt(m, party);
        field.store(result.ct.noise);
        return result;
    }

    int64_t decrypt(const CT& ct) {
        return engine.decrypt(ct.ct);
    }

    CT add(const CT& a, const CT& b) {
        cycles.fetch_add(1);
        CT result;
        result.ct.party_id = a.ct.party_id;
        result.ct.operations = a.ct.operations + b.ct.operations + 1;
        
        // Expand → Blind Add → Contract
        double ea = expand_dim0(a.ct);
        double eb = expand_dim0(b.ct);
        double expanded_sum = ea + eb - LAMBDA;
        
        result.ct.expanded_dim0 = expanded_sum;
        engine.recontract_dim0(result.ct);
        
        // Merge security dimensions
        for(int d = 1; d < godcode::DIMS; d++) {
            result.ct.coordinates[d] = a.ct.coordinates[d] * PHI_INV 
                                      + b.ct.coordinates[d] * (1.0 - PHI_INV);
        }
        
        result.ct.noise = a.ct.noise * PHI_INV + b.ct.noise * (1.0 - PHI_INV);
        field.store(result.ct.noise);
        
        return result;
    }

    CT multiply(const CT& a, const CT& b) {
        cycles.fetch_add(1);
        CT result;
        result.ct.party_id = a.ct.party_id;
        result.ct.operations = a.ct.operations + b.ct.operations + 1;
        
        // Expand → Blind Multiply → Contract
        double ea = expand_dim0(a.ct);
        double eb = expand_dim0(b.ct);
        double expanded_mul = (ea * eb - LAMBDA * (ea + eb) 
                               + LAMBDA * LAMBDA) / PHI + LAMBDA;
        
        result.ct.expanded_dim0 = expanded_mul;
        engine.recontract_dim0(result.ct);
        
        for(int d = 1; d < godcode::DIMS; d++) {
            result.ct.coordinates[d] = a.ct.coordinates[d] * PHI_INV 
                                      + b.ct.coordinates[d] * (1.0 - PHI_INV);
        }
        
        result.ct.noise = (a.ct.noise + b.ct.noise) * PHI_INV + FLOOR * (1.0 - PHI_INV);
        field.store(result.ct.noise);
        
        return result;
    }

    uint64_t total_cycles() const { return cycles.load(); }
    double noise() const { return field.load(); }

private:
    double expand_dim0(const godcode::NDimCiphertext& ct) const {
        double value = ct.coordinates[0];
        int party = ct.party_id;
        for(int layer = DEPTH - 1; layer >= 0; layer--) {
            value -= engine.compute_perturbation(0, layer, party);
            value = (value - FLOOR * (1.0 - PHI_INV)) / PHI_INV;
        }
        return value;
    }
};

} // namespace pozd
