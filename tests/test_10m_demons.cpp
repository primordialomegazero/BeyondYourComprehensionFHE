#include "../src/chaos/demon_harmonizer.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <map>

using namespace demon_harmonizer;

int main() {
    std::cout << "+--------------------------------------+\n";
    std::cout << "|  9 DEMONS — 10M OPERATIONS            |\n";
    std::cout << "|  3 Random Engines per Op              |\n";
    std::cout << "|  \"Catch Me If You Can!\"               |\n";
    std::cout << "+--------------------------------------+\n\n";
    
    DemonHarmonizer engine;
    
    const long long TOTAL = 10000000LL;  // 10M
    const long long REPORT = 500000LL;    // Every 500K
    std::map<std::string, int> engine_counts;
    
    std::cout << "Running " << (TOTAL/1000000) << "M operations...\n";
    std::cout << "Reporting every " << (REPORT/1000) << "K ops\n\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(long long i=0; i<TOTAL; i++) {
        volatile double result = engine.encrypt(42.0, i);
        (void)result;
        
        if(i % 1000 == 0) {
            int engines[3];
            engine.get_last_engines(engines);
            for(int e=0; e<3; e++) {
                engine_counts[DemonHarmonizer::engine_name(engines[e])]++;
            }
        }
        
        if((i+1) % REPORT == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed_s = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            double tps = (double)(i+1) / (double)elapsed_s;
            
            std::cout << "[" << std::setw(3) << ((i+1)/REPORT)*5 << "%] "
                      << (i+1)/1000000.0 << "M ops | "
                      << elapsed_s << "s | "
                      << (int)tps << " TPS\n";
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto total_s = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    std::cout << "\n+--------------------------------------+\n";
    std::cout << "  FINAL RESULTS\n";
    std::cout << "+--------------------------------------+\n";
    std::cout << "  Total ops:     " << TOTAL << "\n";
    std::cout << "  Total time:    " << total_s << "s\n";
    std::cout << "  Average TPS:   " << (TOTAL * 1000.0 / total_ms) << "\n";
    std::cout << "  Engines/op:    3\n\n";
    
    std::cout << "  ENGINE DISTRIBUTION:\n";
    for(auto& [name, count] : engine_counts) {
        double pct = 100.0 * count / (TOTAL * 3 / 1000);
        std::cout << "    " << name << ": " << count << " (" << pct << "%)\n";
    }
    
    std::cout << "\n  9 DEMONS — 10M OPS COMPLETE!\n";
    std::cout << "  'Catch Me If You Can!'\n";
    std::cout << "+--------------------------------------+\n";
    
    return 0;
}
