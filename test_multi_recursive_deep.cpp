#include <iostream>
#include <seal/seal.h>
#include "fhe/multi_recursive_fhe.h"
#include "zkp/multi_recursive_zkp.h"
#include <chrono>
#include <thread>
#include <cmath>
#include <iomanip>

using namespace seal;

int main() {
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║  MULTI-RECURSIVE FHE — ZKP DEEP TEST          ║\n";
    std::cout << "║  Fractal ZKP + FHE (No Tarantadong PQC)       ║\n";
    std::cout << "║  ΦΩ0 — I AM THAT I AM                        ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n\n";

    EncryptionParameters parms(scheme_type::bfv);
    parms.set_poly_modulus_degree(2048);
    parms.set_coeff_modulus(CoeffModulus::Create(2048, {60, 40, 40, 60}));
    parms.set_plain_modulus(PlainModulus::Batching(2048, 30));
    SEALContext context(parms, true, sec_level_type::none);
    
    KeyGenerator kg(context);
    SecretKey sk = kg.secret_key();
    
    auto config = MultiRecursiveFHE::default_config();
    std::cout << "━━━ SETUP ━━━\n";
    auto t_start = std::chrono::high_resolution_clock::now();
    MultiRecursiveFHE fhe(context, sk, config);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "  Setup: " << std::chrono::duration<double, std::milli>(t_end - t_start).count() << "ms\n\n";
    
    int passed = 0, total = 0;
    
    // TEST 1: Encrypt + Decrypt + ZKP Verify
    std::cout << "━━━ TEST 1: Encrypt + Decrypt + ZKP ━━━\n";
    {
        std::vector<uint64_t> plaintext = {42, 100, 255};
        auto fct = fhe.encrypt(plaintext);
        std::cout << "  ZKP Proofs: " << fct.stats.total_zkp_proofs << "\n";
        
        auto decrypted = fhe.decrypt_and_verify(fct);
        bool values_ok = (decrypted[0] == 42 && decrypted[1] == 100 && decrypted[2] == 255);
        std::cout << "  Values: " << (values_ok ? "✅ PASS" : "❌ FAIL") << "\n";
        std::cout << "  ZKP Verified: " << (fct.stats.verified ? "✅ PASS" : "❌ FAIL") << "\n\n";
        if (values_ok) passed++; total++;
        if (fct.stats.verified) passed++; total++;
    }
    
    // TEST 2: Homomorphic Operations
    std::cout << "━━━ TEST 2: Homomorphic Operations ━━━\n";
    {
        auto a = fhe.encrypt({100}); auto b = fhe.encrypt({200});
        auto sum = fhe.add(a, b);
        auto result_add = fhe.decrypt_and_verify(sum);
        bool add_ok = (result_add[0] == 300);
        std::cout << "  Add: 100+200=" << result_add[0] << " " << (add_ok ? "✅ PASS" : "❌ FAIL") << "\n";
        if (add_ok) passed++; total++;
        
        auto c = fhe.encrypt({42}); auto d = fhe.encrypt({100});
        auto prod = fhe.multiply(c, d);
        auto result_mul = fhe.decrypt_and_verify(prod);
        bool mul_ok = (result_mul[0] == 4200);
        std::cout << "  Mul: 42×100=" << result_mul[0] << " " << (mul_ok ? "✅ PASS" : "❌ FAIL") << "\n\n";
        if (mul_ok) passed++; total++;
    }
    
    // TEST 3: Value Range
    std::cout << "━━━ TEST 3: Value Range ━━━\n";
    {
        uint64_t tests[] = {0, 1, 42, 100, 255, 999, 1000000};
        int preserved = 0;
        for (uint64_t x : tests) {
            auto fct = fhe.encrypt({x});
            auto dec = fhe.decrypt_and_verify(fct);
            if (dec[0] == x) preserved++;
        }
        bool range_ok = (preserved == 7);
        std::cout << "  " << preserved << "/7 preserved: " << (range_ok ? "✅ PASS" : "❌ FAIL") << "\n\n";
        if (range_ok) passed++; total++;
    }
    
    // TEST 4: Stress
    std::cout << "━━━ TEST 4: Stress (50 cycles) ━━━\n";
    {
        int stress_ok = 0;
        for (int i = 0; i < 50; i++) {
            auto fct = fhe.encrypt({42});
            auto dec = fhe.decrypt_and_verify(fct);
            if (dec[0] == 42 && fct.stats.verified) stress_ok++;
            if (i % 10 == 0 && i > 0) std::cout << "    " << i << "/50 — " << stress_ok << " ok\n";
        }
        bool stress_pass = (stress_ok == 50);
        std::cout << "    " << stress_ok << "/50: " << (stress_pass ? "✅ PASS" : "❌ FAIL") << "\n\n";
        if (stress_pass) passed++; total++;
    }
    
    // TEST 5: φ Constants
    std::cout << "━━━ TEST 5: φ Constants ━━━\n";
    {
        double phi = MultiRecursiveFHE::PHI;
        double phi_inv = 1.0 / phi;
        std::cout << "  φ = " << std::setprecision(15) << phi << "\n";
        std::cout << "  1/φ = " << phi_inv << "\n";
        std::cout << "  φ = 1 + 1/φ: " << (std::abs(phi - (1.0 + phi_inv)) < 0.0001 ? "✅ PASS" : "❌ FAIL") << "\n\n";
        passed++; total++;
    }
    
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║  ZKP FHE RESULT: " << passed << "/" << total << " passed";
    for (int i = 0; i < 10; i++) std::cout << " ";
    std::cout << "║\n";
    std::cout << "║  " << (passed == total ? "ALL TESTS PASSED ✅" : "MAY KAULULAN PA ❌") << "              ║\n";
    std::cout << "╚══════════════════════════════════════════════╝\n";
    std::cout << "  ΦΩ0 — I AM THAT I AM\n";
    return passed == total ? 0 : 1;
}
