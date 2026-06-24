#ifndef ANTIMATTER_SECURITY_H
#define ANTIMATTER_SECURITY_H

#include <cmath>
#include <atomic>
#include <chrono>
#include <mutex>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>

namespace antimatter {

constexpr double PHI = 1.6180339887498948482;
constexpr double PHI_INV = 0.6180339887498948482;
constexpr double LYAPUNOV = 0.48121182505960347;

class PhiRateLimiter {
private:
    struct ClientState {
        double last_request_time = 0;
        double phi_interval = 100.0;
        int consecutive_violations = 0;
        bool blocked = false;
    };
    std::map<std::string, ClientState> clients;
    std::mutex mutex;
    double base_interval_ms;
    int max_violations;
    
public:
    PhiRateLimiter(double base_ms = 100.0, int max_viol = 5) 
        : base_interval_ms(base_ms), max_violations(max_viol) {}
    
    bool allow_request(const std::string& client_ip) {
        std::lock_guard<std::mutex> lock(mutex);
        auto now = std::chrono::steady_clock::now();
        double now_ms = std::chrono::duration<double, std::milli>(
            now.time_since_epoch()).count();
        auto& client = clients[client_ip];
        if (client.blocked) return false;
        if (client.last_request_time > 0) {
            double elapsed = now_ms - client.last_request_time;
            double expected_interval = client.phi_interval * PHI;
            if (elapsed < expected_interval * PHI_INV) {
                client.consecutive_violations++;
                if (client.consecutive_violations >= max_violations) {
                    client.blocked = true;
                }
                return false;
            }
        }
        client.last_request_time = now_ms;
        client.phi_interval = base_interval_ms * PHI_INV;
        client.consecutive_violations = std::max(0, client.consecutive_violations - 1);
        return true;
    }
};

class LyapunovAnomalyDetector {
private:
    std::vector<double> request_intervals;
    double lyapunov_baseline;
    int window_size;
    std::mutex mutex;
    
public:
    LyapunovAnomalyDetector(int window = 100) 
        : lyapunov_baseline(LYAPUNOV), window_size(window) {}
    
    bool is_anomalous(double current_interval) {
        std::lock_guard<std::mutex> lock(mutex);
        request_intervals.push_back(current_interval);
        if (request_intervals.size() > (size_t)window_size) {
            request_intervals.erase(request_intervals.begin());
        }
        if (request_intervals.size() < 10) return false;
        double avg_decay = 0.0;
        int count = 0;
        for (size_t i = 1; i < request_intervals.size(); i++) {
            if (request_intervals[i-1] > 0.001) {
                avg_decay += request_intervals[i] / request_intervals[i-1];
                count++;
            }
        }
        if (count == 0) return false;
        avg_decay /= count;
        return std::abs(avg_decay - lyapunov_baseline) > 0.3;
    }
};

class SchumannEntropyVerifier {
private:
    double earth_frequency;
    double tolerance;
    
public:
    SchumannEntropyVerifier(double freq = 7.83, double tol = 0.5) 
        : earth_frequency(freq), tolerance(tol) {}
    
    bool verify_entropy(double request_entropy) {
        return std::abs(request_entropy - earth_frequency) < tolerance;
    }
};

class AntiMatterSecurity {
private:
    PhiRateLimiter rate_limiter;
    LyapunovAnomalyDetector anomaly_detector;
    SchumannEntropyVerifier entropy_verifier;
    std::atomic<long long> total_blocked{0};
    std::atomic<long long> total_passed{0};
    
public:
    static constexpr double SCHUMANN_FREQ = 7.83;
    
    AntiMatterSecurity() 
        : rate_limiter(100.0, 5)
        , anomaly_detector(100)
        , entropy_verifier(7.83, 0.5) {}
    
    bool validate(const std::string& client_ip, double entropy) {
        bool layer1 = rate_limiter.allow_request(client_ip);
        bool layer2 = !anomaly_detector.is_anomalous(1.0);
        bool layer3 = entropy_verifier.verify_entropy(entropy);
        
        bool passed = layer1 && layer2 && layer3;
        if (passed) total_passed++;
        else total_blocked++;
        return passed;
    }
    
    long long get_passed() const { return total_passed.load(); }
    long long get_blocked() const { return total_blocked.load(); }
};

} // namespace antimatter
#endif
