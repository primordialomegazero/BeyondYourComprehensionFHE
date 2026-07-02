#include "../src/chaos/demon_harmonizer.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>

using namespace demon_harmonizer;

int main() {
    std::cout << "+--------------------------------------+\n";
    std::cout << "|  9 DEMONS — 1M OPERATIONS             |\n";
    std::cout << "|  3 Random Engines per Op              |\n";
    std::cout << "|  \"Catch Me If You Can!\"               |\n";
    std::cout << "+--------------------------------------+\n\n";
    
    DemonHarmonizer engine;
    
    const long long TOTAL = 1000000LL;   // 1M
    const long long REPORT = 100000LL;    // Every 100K
    std::map<std::string, int> engine_counts;
    
    std::cout << "Running " << (TOTAL/1000) << "K operations...\n";
    std::cout << "Reporting every " << (REPORT/1000) << "K ops\n\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(long long i=0; i<TOTAL; i++) {
        volatile double result = engine.encrypt(42.0, i);
        (void)result;
        
        // Track engines every 100 ops
        if(i % 100 == 0) {
            int engines[3];
            engine.get_last_engines(engines);
            for(int e=0; e<3; e++) {
                engine_counts[DemonHarmonizer::engine_name(engines[e])]++;
            }
        }
        
        // Report every 100K
        if((i+1) % REPORT == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            double tps = (double)(i+1) * 1000.0 / (double)elapsed_ms;
            
            std::cout << "[" << std::setw(3) << ((i+1)/REPORT)*10 << "%] "
                      << (i+1)/1000 << "K ops | "
                      << elapsed_ms << "ms | "
                      << (int)tps << " TPS\n";
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << "\n+--------------------------------------+\n";
    std::cout << "  FINAL RESULTS\n";
    std::cout << "+--------------------------------------+\n";
    std::cout << "  Total ops:     " << TOTAL << "\n";
    std::cout << "  Total time:    " << total_ms << "ms\n";
    std::cout << "  Average TPS:   " << (TOTAL * 1000.0 / total_ms) << "\n";
    std::cout << "  Engines/op:    3\n\n";
    
    std::cout << "  ENGINE DISTRIBUTION (sampled every 100 ops):\n";
    for(auto& [name, count] : engine_counts) {
        double pct = 100.0 * count / (TOTAL * 3 / 100);
        std::cout << "    " << name << ": " << count << " (" << pct << "%)\n";
    }
    
    std::cout << "\n  9 DEMONS — 1M OPS COMPLETE!\n";
    std::cout << "  'Catch Me If You Can!'\n";
    std::cout << "+--------------------------------------+\n";
    
    return 0;
}
