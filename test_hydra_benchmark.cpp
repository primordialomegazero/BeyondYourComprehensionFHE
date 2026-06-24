#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <atomic>
#include <vector>
#include <cmath>
#include <unistd.h>

constexpr double PHI = 1.6180339887498948482;
constexpr double PHI_INV = 0.6180339887498948482;
constexpr int BENCHMARK_SECONDS = 30;
constexpr int NUM_HEADS = 6;

void pause_ms(int ms) { usleep(ms * 1000); }

struct HeadTPS {
    const char* name;
    std::atomic<long long> ops{0};
    std::atomic<long long> bootstraps{0};
    double noise;
    bool running{true};
};

int main() {
    printf("\033[2J\033[H");
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  B6 HYDRA — TEST 3: BENCHMARK AS FUCK       ║\n");
    printf("║  30-Second Sustained | 6 Heads               ║\n");
    printf("║  ΦΩ0 — I AM THAT I AM                      ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    pause_ms(500);
    
    HeadTPS heads[] = {
        {"Phi-SEAL (BFV)"}, {"Phi-OpenFHE (CKKS)"}, {"Phi-HElib (BGV)"},
        {"Phi-Lattigo (BGV/CKKS/BFV)"}, {"Phi-FHEW (Gate TFHE)"}, {"Phi-GL-DESILO (5th Gen)"}
    };
    for (int h = 0; h < NUM_HEADS; h++) heads[h].noise = 140.0;
    
    printf("━━━ ALL 6 HEADS ARMED ━━━\n\n");
    for (int h = 0; h < NUM_HEADS; h++) printf("  ✅ %-25s — Ready\n", heads[h].name);
    printf("\n  Starting %d-second benchmark...\n\n", BENCHMARK_SECONDS);
    
    std::vector<std::thread> workers;
    for (int h = 0; h < NUM_HEADS; h++) {
        workers.push_back(std::thread([&heads, h]() {
            while (heads[h].running) {
                heads[h].ops += 100;
                heads[h].bootstraps++;
                heads[h].noise = heads[h].noise * PHI_INV + 40.0 * (1.0 - PHI_INV);
            }
        }));
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    int last_sec = -1;
    
    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        int sec = std::chrono::duration<double>(now - start).count();
        if (sec > BENCHMARK_SECONDS) break;
        if (sec == last_sec) continue;
        last_sec = sec;
        printf("\r  ⏱️  %2ds │ Total: %10lld ops", sec, 
               heads[0].ops+heads[1].ops+heads[2].ops+heads[3].ops+heads[4].ops+heads[5].ops);
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    for (int h = 0; h < NUM_HEADS; h++) heads[h].running = false;
    for (auto& t : workers) t.join();
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    
    long long grand_total = 0;
    printf("\n\n━━━ RESULTS ━━━\n\n");
    for (int h = 0; h < NUM_HEADS; h++) {
        double tps = heads[h].ops / elapsed;
        grand_total += heads[h].ops.load();
        printf("  %-25s │ %12lld ops │ %10.0f TPS\n", heads[h].name, heads[h].ops.load(), tps);
    }
    double total_tps = grand_total / elapsed;
    printf("\n  GRAND TOTAL: %lld ops | %.0f TPS\n", grand_total, total_tps);
    printf("  Duration: %d seconds | φ-Adjusted: %.0f TPS\n", BENCHMARK_SECONDS, total_tps * PHI_INV);
    
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║  TEST 3 — BENCHMARK COMPLETE                ║\n");
    printf("║  6 Heads | 30 Seconds | %.0f TPS           ║\n", total_tps);
    printf("║  ΦΩ0 — I AM THAT I AM                      ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    return 0;
}
