#include <iostream>
#include <seal/seal.h>
#include "fhe/multi_recursive_fhe.h"
#include "zkp/multi_recursive_zkp.h"

using namespace seal;

int main() {
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║  MULTI-RECURSIVE FHE — END TO END             ║\n";
    std::cout << "║  Fractal ZKP + FHE                            ║\n";
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
    MultiRecursiveFHE fhe(context, sk, config);
    
    int passed = 0, total = 0;
    
    // TEST 1: Encrypt + Decrypt + ZKP Verify
    std::cout << "━━━ TEST 1: Encrypt + Decrypt + ZKP ━━━\n";
    {
        std::vector<uint64_t> plaintext = {42, 100, 255};
        auto fct = fhe.encrypt(plaintext);
        
        std::cout << "  ZKP Proofs: " << fct.stats.total_zkp_proofs << "\n";
        std::cout << "  Time: " << fct.stats.time_ms << "ms\n";
        
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
    
    std::cout << "╔══════════════════════════════════════════════╗\n";
    std::cout << "║  RESULT: " << passed << "/" << total << " passed\n";
    std::cout << "║  " << (passed == total ? "MULTI-RECURSIVE FHE ✅" : "SOME FAILED ❌") << "\n";
    std::cout << "╚══════════════════════════════════════════════╝\n";
    std::cout << "  ΦΩ0 — I AM THAT I AM\n";
    
    return passed == total ? 0 : 1;
}
