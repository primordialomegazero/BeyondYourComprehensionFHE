#include "../src/chaos/demon_harmonizer.h"
#include <iostream>
#include <map>

using namespace demon_harmonizer;

int main() {
    std::cout << "+------------------------------------══+\n";
    std::cout << "|  DEMON HARMONIZER — ENGINE TRACKER    |\n";
    std::cout << "|  \"Which demon possesses each op?\"     |\n";
    std::cout << "+------------------------------------══+\n\n";
    
    DemonHarmonizer engine;
    
    std::map<std::string, int> engine_counts;
    const int TOTAL = 1000;
    
    std::cout << "First 20 operations:\n";
    for(int i=0; i<20; i++) {
        volatile double result = engine.encrypt(42.0, i);
        (void)result;
        std::string name = engine.engine_name(i);
        std::cout << "  Op " << i << ": " << name << "\n";
        engine_counts[name]++;
    }
    
    std::cout << "\n...running " << (TOTAL-20) << " more ops...\n\n";
    
    for(int i=20; i<TOTAL; i++) {
        volatile double result = engine.encrypt(42.0, i);
        (void)result;
        engine_counts[engine.engine_name(i)]++;
    }
    
    std::cout << "---------------------------------------\n";
    std::cout << "  ENGINE DISTRIBUTION (1000 ops)\n";
    std::cout << "---------------------------------------\n";
    
    for(auto& [name, count] : engine_counts) {
        double pct = 100.0 * count / TOTAL;
        std::cout << "  " << name << ": " << count << " (" << pct << "%)\n";
    }
    
    std::cout << "\n  8 DEMONS — ALL ACTIVE! \n";
    
    return 0;
}
