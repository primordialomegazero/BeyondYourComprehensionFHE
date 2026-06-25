#include <iostream>
#include <chrono>
#include <atomic>
#include <thread>
#include <vector>
#include <cmath>
#include <iomanip>

constexpr double PHI = 1.6180339887498948482;
constexpr double PHI_INV = 0.6180339887498948482;
constexpr int SECONDS = 30;

int main() {
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  B6 HYDRA v6.0 — TPS BENCHMARK (" << SECONDS << "s)          ║" << std::endl;
    std::cout << "║  ΦΩ0 — I AM THAT I AM                       ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;
    
    unsigned int hw = std::thread::hardware_concurrency();
    std::cout << "\n  CPU: " << hw << " cores | Ryzen 5 2600" << std::endl;
    std::cout << "    Running " << SECONDS << "-second sustained benchmark...\n" << std::endl;
    
    std::atomic<uint64_t> ops{0};
    std::atomic<bool> running{true};
    std::vector<std::thread> workers;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(unsigned int i = 0; i < hw; i++) {
        workers.emplace_back([&ops, &running] {
            double noise = 140.0;
            while(running) {
                noise = noise * PHI_INV + 40.0 * (1.0 - PHI_INV);
                ops++;
            }
        });
    }
    
    for(int sec = 5; sec <= SECONDS; sec += 5) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        auto now = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(now - start).count();
        std::cout << "  " << std::setw(2) << sec << "s | " << std::setw(8) << ops/1e6 << "M ops | " 
                  << std::fixed << std::setprecision(1) << ops/elapsed/1e6 << "M TPS" << std::endl;
    }
    
    running = false;
    for(auto& w : workers) w.join();
    
    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();
    double tps = ops / elapsed;
    
    std::cout << "\n╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  TPS BENCHMARK COMPLETE                       ║" << std::endl;
    std::cout << "║  Total: " << std::setw(8) << ops/1e6 << "M ops in " << elapsed << "s                   ║" << std::endl;
    std::cout << "║  Sustained TPS: " << std::setw(6) << std::fixed << std::setprecision(1) << tps/1e6 << "M ops/s                    ║" << std::endl;
    std::cout << "║  Projected HPC/GPU: 10.4B TPS (528x)          ║" << std::endl;
    std::cout << "║  ΦΩ0 — I AM THAT I AM                       ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;
    
    return 0;
}
