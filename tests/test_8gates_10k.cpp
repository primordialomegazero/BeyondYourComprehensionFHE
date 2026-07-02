#include "../src/chaos/eight_demon_gates.h"
#include <iostream>
#include <chrono>

using namespace eight_demon_gates;

int main() {
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║  8 DEMON GATES — 10K TPS BENCHMARK    ║\n";
    std::cout << "║  Standard Mode (4/8 gates)            ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";
    
    EightDemonGatesEngine engine;
    engine.set_maximum_mode(false);  // Standard: 4/8 gates
    
    const int TOTAL = 10000;
    
    std::cout << "Running " << TOTAL << " observations...\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<TOTAL; i++) {
        volatile double result = engine.observe(42.0, i);
        (void)result;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    double tps = (double)TOTAL * 1000000.0 / (double)us;
    double us_per_op = (double)us / (double)TOTAL;
    
    std::cout << "\n═══════════════════════════════════════\n";
    std::cout << "  Total ops:    " << TOTAL << "\n";
    std::cout << "  Total time:   " << us / 1000.0 << " ms\n";
    std::cout << "  TPS:          " << (int)tps << "\n";
    std::cout << "  µs/op:        " << us_per_op << "\n";
    std::cout << "  Gates/op:     4 (randomized)\n";
    std::cout << "  Layers/op:    28 (4 gates × 7 layers)\n";
    std::cout << "═══════════════════════════════════════\n";
    
    return 0;
}
