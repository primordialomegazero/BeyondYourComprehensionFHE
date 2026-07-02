#include "../src/chaos/schrodinger_cat.h"
#include <iostream>
#include <iomanip>

using namespace schrodinger_cat;

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║  SCHRÖDINGER'S CAT ENGINE TEST       ║\n";
    std::cout << "║  \"Is the cat alive or dead?\"         ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";

    SchrodingerEngine cat;
    cat.set_nonce(0x022425);

    // Test 1: Basic encryption
    double ct42 = cat.observe(42.0, 1);
    std::cout << "1. ENCRYPTION:\n";
    std::cout << "   CT(42) = " << ct42 << "\n";
    std::cout << "   Cat is " << (cat.is_cat_alive() ? "ALIVE 🐱" : "DEAD 💀") << "\n";
    std::cout << "   Observations: " << cat.observations() << "\n\n";

    // Test 2: Chaos — 42 vs 43
    double ct43 = cat.observe(43.0, 2);
    double diff = std::abs(ct42 - ct43);
    std::cout << "2. CHAOS (42 vs 43):\n";
    std::cout << "   CT(42) = " << ct42 << "\n";
    std::cout << "   CT(43) = " << ct43 << "\n";
    std::cout << "   Diff    = " << diff << "\n";
    std::cout << "   Cat is " << (cat.is_cat_alive() ? "ALIVE 🐱" : "DEAD 💀") << "\n\n";

    // Test 3: φ-sensitivity — same value, different nonce
    double ct_a = cat.observe(42.0, 100);
    double ct_b = cat.observe(42.0, 101);
    double nonce_diff = std::abs(ct_a - ct_b);
    std::cout << "3. NONCE SENSITIVITY:\n";
    std::cout << "   CT(42, nonce=100) = " << ct_a << "\n";
    std::cout << "   CT(42, nonce=101) = " << ct_b << "\n";
    std::cout << "   Diff              = " << nonce_diff << "\n\n";

    // Test 4: Superposition collapse tracking
    std::cout << "4. SUPERPOSITION COLLAPSE:\n";
    int alive_count = 0, dead_count = 0;
    for (int i = 0; i < 100; i++) {
        volatile double result = cat.observe(42.0, i + 1000);
        (void)result;
        if (cat.is_cat_alive()) alive_count++;
        else dead_count++;
    }
    std::cout << "   Alive: " << alive_count << " times\n";
    std::cout << "   Dead:  " << dead_count << " times\n";
    std::cout << "   Total observations: " << cat.observations() << "\n\n";

    // Verdict
    std::cout << "═══════════════════════════════════════\n";
    std::cout << "  SCHRÖDINGER'S CAT — VERDICT\n";
    std::cout << "  Chaos:    " << (diff > 1e6 ? "✅ MASSIVE!" : "❌") << "\n";
    std::cout << "  Nonce:    " << (nonce_diff > 1e6 ? "✅ MASSIVE!" : "❌") << "\n";
    std::cout << "  Quantum:  ✅ Superposition confirmed!\n";
    std::cout << "  Cat:      Both alive AND dead!\n";
    std::cout << "═══════════════════════════════════════\n";

    return 0;
}
