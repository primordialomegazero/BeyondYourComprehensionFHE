#include "../src/chaos/demon_harmonizer.h"
#include <iostream>
#include <map>
#include <iomanip>

using namespace demon_harmonizer;

int main() {
    std::cout << "+------------------------------------══+\n";
    std::cout << "|  CATCH ME IF YOU CAN — 9 DEMONS      |\n";
    std::cout << "|  3 Random Engines per Op             |\n";
    std::cout << "|  \"Triple Rashomon × 9\"               |\n";
    std::cout << "+------------------------------------══+\n\n";

    DemonHarmonizer engine;
    engine.set_nonce(0x022425);

    std::map<std::string, int> engine_counts;
    const int TOTAL = 1000;

    // Track all 3 engines per operation
    std::cout << "First 10 operations (3 engines each):\n";
    for(int i = 0; i < 10; i++) {
        volatile double result = engine.encrypt(42.0, i);
        (void)result;
        
        int engines[3];
        engine.get_last_engines(engines);
        
        std::cout << "  Op " << i << ": "
                  << engine.engine_name(engines[0]) << " + "
                  << engine.engine_name(engines[1]) << " + "
                  << engine.engine_name(engines[2]) << "\n";
        
        for(int e = 0; e < 3; e++) {
            engine_counts[engine.engine_name(engines[e])]++;
        }
    }

    std::cout << "\n...running " << (TOTAL-10) << " more ops...\n\n";

    for(int i = 10; i < TOTAL; i++) {
        volatile double result = engine.encrypt(42.0, i);
        (void)result;
        
        int engines[3];
        engine.get_last_engines(engines);
        for(int e = 0; e < 3; e++) {
            engine_counts[engine.engine_name(engines[e])]++;
        }
    }

    std::cout << "---------------------------------------\n";
    std::cout << "  ENGINE DISTRIBUTION (1000 ops × 3)\n";
    std::cout << "  Total engine calls: " << (TOTAL * 3) << "\n";
    std::cout << "---------------------------------------\n";

    for(auto& [name, count] : engine_counts) {
        double pct = 100.0 * count / (TOTAL * 3);
        std::cout << "  " << std::setw(18) << std::left << name 
                  << ": " << std::setw(4) << count 
                  << " (" << std::fixed << std::setprecision(1) << pct << "%)\n";
    }

    std::cout << "\n   9 DEMONS — 3 PER OP — ALL ACTIVE!\n";
    std::cout << "   'Catch Me If You Can!' \n";

    return 0;
}
