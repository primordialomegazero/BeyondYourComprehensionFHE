#include <iostream>
#include <thread>
#include <chrono>
#include "phi_drogon.h"

int main() {
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║  Φ-DROGON — RECURSIVE FRACTAL THREAT TEST  ║\n");
    printf("║  φ-Harmonic Thread Pool + Threat Detection  ║\n");
    printf("║  ΦΩ0 — I AM THAT I AM                      ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    phi_drogon::RecursiveFractalThreadPool pool;
    
    printf("━━━ PHASE 1: FRACTAL THREAD HIERARCHY ━━━\n");
    pool.print_status();
    
    printf("\n━━━ PHASE 2: SIMULATING REQUESTS ━━━\n");
    printf("  Processing 1000 requests with φ-weighted balancing...\n");
    
    for (int i = 0; i < 1000; i++) {
        int thread_id = pool.get_optimal_thread();
        if (i % 200 == 0) {
            printf("  Request %3d → Thread %d (φ-harmonic)\n", i, thread_id);
        }
    }
    
    printf("  ✅ 1000 requests processed!\n");
    
    printf("\n━━━ PHASE 3: THREAT DETECTION ━━━\n");
    bool threat = pool.detect_threats();
    printf("  Threat scan: %s\n", threat ? "⚠️ THREAT DETECTED!" : "✅ CLEAN");
    
    printf("\n━━━ PHASE 4: HARMONIZATION ━━━\n");
    pool.harmonize();
    printf("  ✅ Thread pool harmonized via φ\n");
    
    printf("\n━━━ FINAL STATUS ━━━\n");
    pool.print_status();
    
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║  ✅ Φ-DROGON — RECURSIVE FRACTAL ACTIVE     ║\n");
    printf("║  φ-Weighted + Lyapunov + Threat Detection   ║\n");
    printf("║  ΦΩ0 — I AM THAT I AM                      ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    return 0;
}
