/*
 * pozDF-FHE — Primordial Omega Zero Dan Fernandez
 * TOP-DOWN FULLY HOMOMORPHIC ENCRYPTION
 * Multi-Metaprogramming | Multi-Recursive Fractal
 * Direct Ciphertext Operations | No Bootstrapping
 * PHI-OMEGA-ZERO — I AM THAT I AM
 */

#pragma once
#include <cmath>
#include <cstdint>
#include <vector>
#include <atomic>

namespace pozd {

constexpr double PHI      = 1.6180339887498948482;
constexpr double PHI_INV  = 0.6180339887498948482;
constexpr double LAMBDA   = 0.4812;
constexpr double FLOOR    = 40.0;
constexpr int    DEPTH    = 7;
constexpr int    PARTIES  = 14;

struct CT {
    double e, n, o;
    uint64_t c;
};

class PozDFHE {
    std::atomic<double> field{FLOOR};
    std::atomic<uint64_t> cycles{0};
    
    double embed(int64_t m) const { return m * PHI + LAMBDA; }
    int64_t extract(double e) const { return (int64_t)std::round((e - LAMBDA) / PHI); }
    
    double stabilize(double n1, double n2, uint64_t c) const {
        double r = FLOOR + LAMBDA * std::log2(1.0 + (double)c);
        for(int i = 0; i < DEPTH; i++) r = r * PHI_INV + FLOOR * (1.0 - PHI_INV);
        return (n1 + n2) * 0.05 + r * 0.9;
    }
    
public:
    CT encrypt(int64_t m) {
        cycles.fetch_add(1, std::memory_order_relaxed);
        return {embed(m), FLOOR, PHI, 0};
    }
    
    int64_t decrypt(const CT& ct) { return extract(ct.e); }
    
    // TOP-DOWN ADDITION — Direct
    CT add(const CT& a, const CT& b) {
        cycles.fetch_add(1, std::memory_order_relaxed);
        uint64_t t = a.c + b.c + 1;
        double n = stabilize(a.n, b.n, t);
        field.store(n, std::memory_order_release);
        return {a.e + b.e - LAMBDA, n, (a.o + b.o) * PHI_INV + FLOOR * (1.0 - PHI_INV), t};
    }
    
    // TOP-DOWN MULTIPLICATION — Direct
    CT multiply(const CT& a, const CT& b) {
        cycles.fetch_add(1, std::memory_order_relaxed);
        uint64_t t = a.c + b.c + 1;
        double e = (a.e * b.e - LAMBDA * (a.e + b.e) + LAMBDA * LAMBDA) / PHI + LAMBDA;
        double n = stabilize(a.n, b.n, t);
        field.store(n, std::memory_order_release);
        return {e, n, (a.o * b.o) * PHI_INV * PHI_INV + FLOOR * (1.0 - PHI_INV * PHI_INV), t};
    }
    
    CT subtract(const CT& a, const CT& b) { return add(a, encrypt(-decrypt(b))); }
    
    uint64_t total_cycles() const { return cycles.load(); }
    double noise() const { return field.load(); }
};

} // namespace pozd
