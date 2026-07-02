#include "../src/chaos/demon_harmonizer.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>

using namespace demon_harmonizer;

int main() {
    std::cout << "+--------------------------------------+\n";
    std::cout << "|  9 DEMONS — 100M OPERATIONS           |\n";
    std::cout << "|  3 Random Engines per Op              |\n";
    std::cout << "|  \"Catch Me If You Can!\"               |\n";
    std::cout << "+--------------------------------------+\n\n";
    
    DemonHarmonizer engine;
    
    const long long TOTAL = 100000000LL;  // 100M
    const long long REPORT = 10000000LL;   // Every 10M
    std::map<std::string, int> engine_counts;
    long long errors = 0;
    
    std::cout << "Running " << (TOTAL/1000000) << "M operations...\n";
    std::cout << "Reporting every " << (REPORT/1000000) << "M ops\n\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    auto batch_start = start;
    
    for(long long i=0; i<TOTAL; i++) {
        volatile double result = engine.encrypt(42.0, i);
        (void)result;
        
        // Track engines every 1000 ops
        if(i % 1000 == 0) {
            int engines[3];
            engine.get_last_engines(engines);
            for(int e=0; e<3; e++) {
                engine_counts[DemonHarmonizer::engine_name(engines[e])]++;
            }
        }
        
        // Report every 10M
        if((i+1) % REPORT == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto batch_us = std::chrono::duration_cast<std::chrono::microseconds>(now - batch_start).count();
            auto total_s = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            
            double batch_tps = (double)REPORT * 1000000.0 / (double)batch_us;
            
            std::cout << "[" << std::setw(3) << ((i+1)/REPORT)*10 << "%] "
                      << (i+1)/1000000 << "M ops | "
                      << batch_us/1000 << "ms batch | "
                      << (int)batch_tps << " TPS | "
                      << total_s << "s total\n";
            
            batch_start = std::chrono::high_resolution_clock::now();
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto total_s = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    auto total_us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    
    std::cout << "\n+--------------------------------------+\n";
    std::cout << "  FINAL RESULTS\n";
    std::cout << "+--------------------------------------+\n";
    std::cout << "  Total ops:     " << TOTAL << "\n";
    std::cout << "  Total time:    " << total_s << "s\n";
    std::cout << "  Average TPS:   " << (TOTAL * 1000000.0 / total_us) << "\n";
    std::cout << "  Engines/op:    3\n";
    std::cout << "  Total engines: " << (TOTAL * 3) << "\n\n";
    
    std::cout << "  ENGINE DISTRIBUTION:\n";
    for(auto& [name, count] : engine_counts) {
        std::cout << "    " << name << ": " << count << "\n";
    }
    
    std::cout << "\n  9 DEMONS — 100M OPS COMPLETE!\n";
    std::cout << "  'Catch Me If You Can!'\n";
    std::cout << "+--------------------------------------+\n";
    
    return 0;
}
