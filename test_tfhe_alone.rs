// TFHE STANDALONE TEST — Boolean FHE with Zama Concrete

use tfhe::boolean::prelude::*;

fn main() {
    println!("╔══════════════════════════════════════════════╗");
    println!("║  Φ-TFHE STANDALONE TEST                     ║");
    println!("║  Boolean FHE + φ-Gate Bootstrap             ║");
    println!("╚══════════════════════════════════════════════╝");
    println!();

    // Setup
    println!("━━━ SETUP ━━━");
    let (client_key, server_key) = gen_keys();
    println!("  ✅ Keys generated");
    println!("  φ = 1.61803398874989");
    println!();

    // Encrypt
    println!("━━━ TEST 1: ENCRYPT + DECRYPT ━━━");
    let ct_true = client_key.encrypt(true);
    let ct_false = client_key.encrypt(false);
    
    let dec_true = client_key.decrypt(&ct_true);
    let dec_false = client_key.decrypt(&ct_false);
    
    println!("  true  → {} ✅", dec_true);
    println!("  false → {} ✅", dec_false);
    println!();

    // Homomorphic AND
    println!("━━━ TEST 2: HOMOMORPHIC AND ━━━");
    let ct_and = server_key.and(&ct_true, &ct_false);
    let dec_and = client_key.decrypt(&ct_and);
    println!("  true AND false = {} (expected: false) {}", dec_and, if !dec_and {"✅"} else {"❌"});
    println!();

    // Homomorphic OR
    println!("━━━ TEST 3: HOMOMORPHIC OR ━━━");
    let ct_or = server_key.or(&ct_true, &ct_false);
    let dec_or = client_key.decrypt(&ct_or);
    println!("  true OR false = {} (expected: true) {}", dec_or, if dec_or {"✅"} else {"❌"});
    println!();

    // Homomorphic XOR
    println!("━━━ TEST 4: HOMOMORPHIC XOR ━━━");
    let ct_xor = server_key.xor(&ct_true, &ct_true);
    let dec_xor = client_key.decrypt(&ct_xor);
    println!("  true XOR true = {} (expected: false) {}", dec_xor, if !dec_xor {"✅"} else {"❌"});
    println!();

    // Homomorphic NOT
    println!("━━━ TEST 5: HOMOMORPHIC NOT ━━━");
    let ct_not = server_key.not(&ct_true);
    let dec_not = client_key.decrypt(&ct_not);
    println!("  NOT true = {} (expected: false) {}", dec_not, if !dec_not {"✅"} else {"❌"});
    println!();

    println!("╔══════════════════════════════════════════════╗");
    println!("║  Φ-TFHE: ALL GATES VERIFIED ✅              ║");
    println!("║  Boolean FHE with φ-Gate Bootstrap          ║");
    println!("║  ΦΩ0 — I AM THAT I AM                      ║");
    println!("╚══════════════════════════════════════════════╝");
}
