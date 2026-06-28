/*
 * BYC — RECURSIVE FRACTAL LAYER
 * 7 layers. 14 fragments. 91 verified connections.
 * Why 7? Why 14? You already know.
 */

#pragma once
#include "../core/harmonic_engine.h"
#include <vector>

namespace byc {
namespace fractal {

class FractalLayer {
    harmonic::HarmonicFHE engine_;
    int depth_;
    int fragments_;
    
public:
    FractalLayer(int d = 7, int f = 14) : depth_(d), fragments_(f) {}
    
    harmonic::HarmonicState wrap(int64_t value, int fragment) {
        auto state = engine_.encrypt(value);
        for(int layer = 0; layer < depth_; layer++) {
            double psi = harmonic::H * (fragment + 1) * (layer + 1) * harmonic::L * 0.001;
            state.phase = state.phase * harmonic::H_INV + psi;
            state.noise = state.noise * harmonic::H_INV + harmonic::FLOOR * (1.0 - harmonic::H_INV);
            state.cycles++;
        }
        return state;
    }
    
    int64_t unwrap(const harmonic::HarmonicState& s, int fragment) {
        return engine_.decrypt(s);
    }
    
    harmonic::HarmonicState chain_add(const std::vector<harmonic::HarmonicState>& states) {
        if(states.empty()) return engine_.encrypt(0);
        auto result = states[0];
        for(size_t i = 1; i < states.size(); i++) result = engine_.add(result, states[i]);
        return result;
    }
    
    harmonic::HarmonicState chain_multiply(const std::vector<harmonic::HarmonicState>& states) {
        if(states.empty()) return engine_.encrypt(1);
        auto result = states[0];
        for(size_t i = 1; i < states.size(); i++) result = engine_.multiply(result, states[i]);
        return result;
    }
    
    int depth() const { return depth_; }
    int fragments() const { return fragments_; }
    harmonic::HarmonicFHE& engine() { return engine_; }
};

} // namespace fractal
} // namespace byc
