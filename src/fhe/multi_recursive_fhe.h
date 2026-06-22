#pragma once

#include <seal/seal.h>
#include "zkp/multi_recursive_zkp.h"
#include <vector>
#include <chrono>
#include <cmath>
#include <iostream>

using namespace seal;

class MultiRecursiveFHE {
public:
    static constexpr double PHI = 1.6180339887498948482;
    static constexpr double PHI_INV = 0.6180339887498948482;
    
    struct FractalConfig {
        size_t recursion_depth;
        size_t zkp_branches;
        size_t fhe_cycles;
        bool use_φ_convergence;
    };
    
    static FractalConfig default_config() {
        return {3, 2, 50, true};
    }
    
    struct FractalStats {
        size_t total_encryptions = 0;
        size_t total_decryptions = 0;
        size_t total_zkp_proofs = 0;
        double time_ms = 0;
        bool homomorphic = true;
        bool verified = false;
    };
    
    struct FractalCiphertext {
        Ciphertext data;
        MultiRecursiveZKP::FractalTree zkp;
        FractalStats stats;
    };
    
    MultiRecursiveFHE(const SEALContext &context, const SecretKey &sk, const FractalConfig &config);
    FractalCiphertext encrypt(const std::vector<uint64_t>& plaintext);
    std::vector<uint64_t> decrypt_and_verify(FractalCiphertext& fct);
    FractalCiphertext add(FractalCiphertext& a, FractalCiphertext& b);
    FractalCiphertext multiply(FractalCiphertext& a, FractalCiphertext& b);

private:
    const SEALContext &context_;
    SecretKey sk_;
    FractalConfig config_;
    BatchEncoder encoder_;
    Decryptor decryptor_;
    Ciphertext enc_zero_;
};
