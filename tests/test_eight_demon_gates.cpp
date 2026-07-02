#include "../src/chaos/eight_demon_gates.h"
#include <iostream>
#include <iomanip>

using namespace eight_demon_gates;

int main() {
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║  8 DEMON GATES — FULL INTEGRATION TEST    ║\n";
    std::cout << "║  \"Overkill as fuck. As intended.\"         ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n\n";
    
    EightDemonGatesEngine engine;
    engine.set_maximum_mode(false);  // Standard: 9 gates
    
    // Test 1: Basic observation
    std::cout << "═══ TEST 1: GATE SELECTION ═══\n";
    auto names1 = engine.active_gate_names(1);
    auto names2 = engine.active_gate_names(2);
    
    std::cout << "Op 1 gates: ";
    for(int i=0; i<4; i++) std::cout << names1[i] << (i<3 ? ", " : "");
    std::cout << "\n";
    
    std::cout << "Op 2 gates: ";
    for(int i=0; i<4; i++) std::cout << names2[i] << (i<3 ? ", " : "");
    std::cout << "\n\n";
    
    // Test 2: Chaos difference (42 vs 43)
    std::cout << "═══ TEST 2: CHAOS SENSITIVITY ═══\n";
    double v42 = engine.observe(42.0, 100);
    double v43 = engine.observe(43.0, 100);
    double diff = std::abs(v42 - v43);
    
    std::cout << "42 → " << std::scientific << v42 << "\n";
    std::cout << "43 → " << v43 << "\n";
    std::cout << "Diff = " << diff << "\n";
    std::cout << (diff > 1e6 ? "✅ MASSIVE CHAOS!" : "⚠️  Weak") << "\n\n";
    
    // Test 3: Nonce sensitivity (same value, different nonce)
    std::cout << "═══ TEST 3: NONCE SENSITIVITY ═══\n";
    double v_a = engine.observe(42.0, 100);
    double v_b = engine.observe(42.0, 200);
    double nonce_diff = std::abs(v_a - v_b);
    
    std::cout << "Nonce 100 → " << std::scientific << v_a << "\n";
    std::cout << "Nonce 200 → " << v_b << "\n";
    std::cout << "Diff = " << nonce_diff << "\n";
    std::cout << (nonce_diff > 1e6 ? "✅ NONCE CHAOS!" : "⚠️  Weak") << "\n\n";
    
    // Test 4: MAXIMUM MODE (all 9 gates)
    std::cout << "═══ TEST 4: MAXIMUM MODE (8/8 GATES) ═══\n";
    engine.set_maximum_mode(true);
    double v_max = engine.observe(42.0, 999);
    std::cout << "42 (MAX) → " << std::scientific << v_max << "\n";
    std::cout << "Total layers: " << (8 * 7) << "\n\n";
    
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║  8 DEMON GATES — TEST COMPLETE            ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";
    
    return 0;
}
