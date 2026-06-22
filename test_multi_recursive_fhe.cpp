#include <iostream>
#include <seal/seal.h>
#include "src/fhe/multi_recursive_fhe.h"
#include "src/zkp/multi_recursive_zkp.h"
#include "src/pqc/multi_recursive_pqc.h"

using namespace seal;
using namespace seal::fractal;

int main() {
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║  MULTI-RECURSIVE FHE — END TO END             ║\n";
    std::cout << "║  Fractal ZKP + Recursive PQC + FHE            ║\n";
    std::cout << "║  ΦΩ0 — I AM THAT I AM                        ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";

    // Setup
    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(2048);
    parms.set_coeff_modulus(CoeffModulus::Create(2048, {60, 40, 40, 60}));
    parms.set_plain_modulus(PlainModulus::Batching(2048, 30));
    SEALContext context(parms, true, sec_level_type::none);
    
    KeyGenerator kg(context);
    SecretKey sk = kg.secret_key();
    
    MultiRecursiveFHE::FractalConfig config;
    config.recursion_depth = 3;  // Keep small for demo
    config.fhe_cycles = 100;
    
    MultiRecursiveFHE fhe(context, sk, config);
    
    int passed = 0, total = 0;
    
    // TEST 1: Encrypt + Decrypt + Verify
    std::cout << "=== TEST 1: Encrypt + Decrypt + Verify ===\n";
    {
        std::vector<uint64_t> plaintext = {42, 100, 255};
        auto fct = fhe.encrypt(plaintext);
        
        std::cout << "  ZKP Proofs: " << fct.stats.total_zkp_proofs << "\n";
        std::cout << "  PQC Nodes: " << fct.stats.total_pqc_nodes << "\n";
        std::cout << "  Noise: " << fct.stats.initial_noise << " -> " 
                  << fct.stats.final_noise << " bits\n";
        std::cout << "  Time: " << fct.stats.time_ms << "ms\n";
        
        auto decrypted = fhe.decrypt_and_verify(fct);
        
        bool values_ok = (decrypted[0] == 42 && decrypted[1] == 100 && decrypted[2] == 255);
        std::cout << "  Values: " << (values_ok ? "✅" : "❌") << "\n";
        std::cout << "  Verified: " << (fct.stats.verified ? "✅" : "❌") << "\n";
        if (values_ok) passed++; total++;
        if (fct.stats.verified) passed++; total++;
    }
    
    // TEST 2: Homomorphic Add
    std::cout << "\n=== TEST 2: Homomorphic Add ===\n";
    {
        auto a = fhe.encrypt({100});
        auto b = fhe.encrypt({200});
        auto sum = fhe.add(a, b);
        auto result = fhe.decrypt_and_verify(sum);
        
        bool add_ok = (result[0] == 300);
        std::cout << "  100 + 200 = " << result[0] << " " << (add_ok ? "✅" : "❌") << "\n";
        if (add_ok) passed++; total++;
    }
    
    // TEST 3: Homomorphic Multiply
    std::cout << "\n=== TEST 3: Homomorphic Multiply ===\n";
    {
        auto a = fhe.encrypt({42});
        auto b = fhe.encrypt({100});
        auto prod = fhe.multiply(a, b);
        auto result = fhe.decrypt_and_verify(prod);
        
        bool mul_ok = (result[0] == 4200);
        std::cout << "  42 × 100 = " << result[0] << " " << (mul_ok ? "✅" : "❌") << "\n";
        if (mul_ok) passed++; total++;
    }
    
    std::cout << "\n╔══════════════════════════════════════════════╗\n";
    std::cout << "║  RESULT: " << passed << "/" << total << " passed\n";
    std::cout << "║  " << (passed == total ? "MULTI-RECURSIVE FHE ✅" : "SOME FAILED ❌") << "\n";
    std::cout << "╚══════════════════════════════════════════════╝\n";
    
    return passed == total ? 0 : 1;
}
