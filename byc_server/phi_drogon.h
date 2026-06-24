#ifndef PHI_DROGON_H
#define PHI_DROGON_H

#include <vector>
#include <cmath>
#include <atomic>
#include <mutex>
#include <memory>

namespace phi_drogon {

constexpr double PHI = 1.6180339887498948482;
constexpr double PHI_INV = 0.6180339887498948482;
constexpr double LYAPUNOV = 0.48121182505960347;
constexpr int FRACTAL_DEPTH = 7;
constexpr int MAX_THREADS = 12;

struct FractalThreadNode {
    int thread_id;
    int fractal_layer;
    double phi_weight;
    double current_load;
    double lyapunov_health;
    std::atomic<long long> requests_handled{0};
    std::atomic<long long> threats_detected{0};
    bool active;
    
    void update_health() {
        lyapunov_health = lyapunov_health * PHI_INV + LYAPUNOV * (1.0 - PHI_INV);
    }
    
    void flag_threat() {
        threats_detected++;
        lyapunov_health = lyapunov_health * 1.5;
    }
    
    bool is_threat_detected() const {
        return std::abs(lyapunov_health - LYAPUNOV) > 0.3;
    }
};

class PhiThreatDetector {
private:
    std::vector<double> noise_samples;
    double baseline_lyapunov;
    int sample_window;
    
public:
    PhiThreatDetector() : baseline_lyapunov(LYAPUNOV), sample_window(100) {}
    
    bool analyze(double request_noise) {
        noise_samples.push_back(request_noise);
        if (noise_samples.size() > (size_t)sample_window) {
            noise_samples.erase(noise_samples.begin());
        }
        if (noise_samples.size() < 10) return false;
        
        double avg_decay = 0.0;
        for (size_t i = 1; i < noise_samples.size(); i++) {
            if (noise_samples[i-1] > 0.001) {
                avg_decay += noise_samples[i] / noise_samples[i-1];
            }
        }
        avg_decay /= (noise_samples.size() - 1);
        return std::abs(avg_decay - baseline_lyapunov) > 0.2;
    }
};

class RecursiveFractalThreadPool {
private:
    std::vector<std::unique_ptr<FractalThreadNode>> threads;
    PhiThreatDetector threat_detector;
    std::mutex pool_mutex;
    double global_phi_anchor;
    
public:
    RecursiveFractalThreadPool() : global_phi_anchor(PHI) {
        int thread_count = 0;
        for (int layer = 0; layer < FRACTAL_DEPTH && thread_count < MAX_THREADS; layer++) {
            int threads_in_layer = layer + 1;
            for (int t = 0; t < threads_in_layer && thread_count < MAX_THREADS; t++) {
                auto node = std::make_unique<FractalThreadNode>();
                node->thread_id = thread_count;
                node->fractal_layer = layer;
                node->phi_weight = pow(PHI, layer);
                node->current_load = 0.0;
                node->lyapunov_health = LYAPUNOV;
                node->active = true;
                threads.push_back(std::move(node));
                thread_count++;
            }
        }
    }
    
    int get_optimal_thread() {
        std::lock_guard<std::mutex> lock(pool_mutex);
        int best_thread = 0;
        double best_score = 1e9;
        for (size_t i = 0; i < threads.size(); i++) {
            if (!threads[i]->active) continue;
            double score = threads[i]->current_load / threads[i]->phi_weight;
            if (score < best_score) { best_score = score; best_thread = i; }
        }
        threads[best_thread]->current_load += PHI_INV;
        threads[best_thread]->requests_handled++;
        threads[best_thread]->update_health();
        return best_thread;
    }
    
    bool detect_threats() {
        std::lock_guard<std::mutex> lock(pool_mutex);
        int threats = 0;
        for (auto& t : threads) {
            if (t->is_threat_detected()) { threats++; t->flag_threat(); }
        }
        return (double)threats / threads.size() > PHI_INV;
    }
    
    void harmonize() {
        std::lock_guard<std::mutex> lock(pool_mutex);
        double total_health = 0.0;
        for (auto& t : threads) total_health += t->lyapunov_health;
        global_phi_anchor = total_health / threads.size() * PHI_INV + LYAPUNOV * (1.0 - PHI_INV);
    }
    
    void print_status() {
        std::lock_guard<std::mutex> lock(pool_mutex);
        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║  Φ-DROGON RECURSIVE FRACTAL THREAD POOL     ║\n");
        printf("╠══════════════════════════════════════════════╣\n");
        printf("║  Threads: %zu | φ-Anchor: %.4f             ║\n", threads.size(), global_phi_anchor);
        for (auto& t : threads) {
            printf("║  T%-2d L%-1d φ^%d │ Load: %.2f │ Health: %.4f %s ║\n",
                   t->thread_id, t->fractal_layer, t->fractal_layer,
                   t->current_load, t->lyapunov_health,
                   t->is_threat_detected() ? "⚠️" : "✅");
        }
        printf("╚══════════════════════════════════════════════╝\n");
    }
};

} // namespace phi_drogon
#endif
