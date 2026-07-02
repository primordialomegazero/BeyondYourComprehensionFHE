#include "../src/chaos/demon_harmonizer.h"
#include <iostream>
#include <chrono>

using namespace demon_harmonizer;

int main() {
    std::cout << "+------------------------------------══+\n";
    std::cout << "|  DEMON HARMONIZER — 10K TPS TEST      |\n";
    std::cout << "|  1 Random Engine per Operation        |\n";
    std::cout << "+------------------------------------══+\n\n";
    
    DemonHarmonizer engine;
    
    const int TOTAL = 10000;
    std::cout << "Running " << TOTAL << " operations...\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i=0; i<TOTAL; i++) {
        volatile double result = engine.encrypt(42.0, i);
        (void)result;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    double tps = (double)TOTAL * 1000000.0 / (double)us;
    
    std::cout << "\n---------------------------------------\n";
    std::cout << "  Total ops:    " << TOTAL << "\n";
    std::cout << "  Total time:   " << us / 1000.0 << " ms\n";
    std::cout << "  TPS:          " << (int)tps << "\n";
    std::cout << "  µs/op:        " << (double)us / TOTAL << "\n";
    std::cout << "  Engines/op:   1 (randomized)\n";
    std::cout << "---------------------------------------\n";
    
    return 0;
}
