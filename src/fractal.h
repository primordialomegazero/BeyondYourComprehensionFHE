/*
 * pozDF-FHE — MULTI-RECURSIVE FRACTAL ENGINE
 * 7 Layers | 14 Fragments | 91 Cross-Verified Pairs
 * Top-Down Fully Homomorphic
 */

#pragma once
#include "pozd_fhe.h"

namespace pozd {

class FractalEngine {
    PozDFHE fhe;
    double seeds[PARTIES];
    
public:
    FractalEngine() {
        for(int i = 0; i < PARTIES; i++)
            seeds[i] = PHI * (i + 1) * PHI_INV + FLOOR * (1.0 - PHI_INV);
    }
    
    CT fractal_encrypt(int64_t value, int party) {
        CT ct = fhe.encrypt(value);
        for(int layer = 0; layer < DEPTH; layer++) {
            double psi = PHI * (party + 1) * (layer + 1) * LAMBDA * 0.001;
            ct.o = ct.o * PHI_INV + psi;
            ct.n = ct.n * PHI_INV + FLOOR * (1.0 - PHI_INV);
            ct.c++;
        }
        return ct;
    }
    
    int64_t fractal_decrypt(const CT& ct) { return fhe.decrypt(ct); }
    
    CT chain_add(const std::vector<CT>& cts) {
        if(cts.empty()) return fhe.encrypt(0);
        CT result = cts[0];
        for(size_t i = 1; i < cts.size(); i++) result = fhe.add(result, cts[i]);
        return result;
    }
    
    CT chain_multiply(const std::vector<CT>& cts) {
        if(cts.empty()) return fhe.encrypt(1);
        CT result = cts[0];
        for(size_t i = 1; i < cts.size(); i++) result = fhe.multiply(result, cts[i]);
        return result;
    }
    
    bool verify_all_pairs() {
        int64_t test = (int64_t)(PHI * 100);
        for(int i = 0; i < PARTIES; i++) {
            for(int j = i + 1; j < PARTIES; j++) {
                auto a = fractal_encrypt(test, i);
                auto b = fractal_encrypt(test, j);
                if(fractal_decrypt(a) != test || fractal_decrypt(b) != test)
                    return false;
            }
        }
        return true;
    }
    
    PozDFHE& engine() { return fhe; }
};

} // namespace pozd
