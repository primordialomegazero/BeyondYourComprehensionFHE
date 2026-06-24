#ifndef HYDRA_GATEWAY_H
#define HYDRA_GATEWAY_H

#include <drogon/drogon.h>
#include <json/json.h>
#include <cmath>
#include <atomic>
#include <chrono>
#include <memory>
#include <vector>

namespace hydra_gateway {

constexpr double PHI = 1.6180339887498948482;
constexpr double PHI_INV = 0.6180339887498948482;
constexpr double LYAPUNOV = 0.48121182505960347;
constexpr int MAX_ENGINES = 6;

struct EngineNode {
    std::string name;
    std::string scheme;
    std::string endpoint;
    bool healthy;
    std::atomic<long long> requests_handled{0};
    std::atomic<long long> errors{0};
    double load_factor;
    double health_score;
};

class HydraGateway {
private:
    std::vector<std::unique_ptr<EngineNode>> engines;
    std::atomic<long long> total_requests{0};
    std::chrono::steady_clock::time_point start_time;
    
    double calculate_health_score(double error_rate, double load) {
        return std::exp(-error_rate * PHI) * (1.0 - load * PHI_INV);
    }
    
    int select_engine() {
        int best = 0;
        double best_score = -1.0;
        for (size_t i = 0; i < engines.size(); i++) {
            if (!engines[i]->healthy) continue;
            double score = engines[i]->health_score / (engines[i]->load_factor + 0.01);
            if (score > best_score) { best_score = score; best = i; }
        }
        return best;
    }
    
public:
    HydraGateway() : start_time(std::chrono::steady_clock::now()) {
        auto add_engine = [this](const char* name, const char* scheme, const char* endpoint) {
            auto node = std::make_unique<EngineNode>();
            node->name = name;
            node->scheme = scheme;
            node->endpoint = endpoint;
            node->healthy = true;
            node->load_factor = 0.0;
            node->health_score = 1.0;
            engines.push_back(std::move(node));
        };
        
        add_engine("Phi-SEAL", "BFV", "http://localhost:8081");
        add_engine("Phi-OpenFHE", "CKKS", "http://localhost:8082");
        add_engine("Phi-HElib", "BGV", "http://localhost:8083");
        add_engine("Phi-Lattigo", "BGV/CKKS/BFV", "http://localhost:8084");
        add_engine("Phi-FHEW", "Gate TFHE", "http://localhost:8085");
        add_engine("Phi-GL-DESILO", "5th Gen FHE", "http://localhost:8086");
    }
    
    Json::Value get_gateway_status() {
        Json::Value status;
        status["gateway"] = "Hydra FHE Gateway";
        status["version"] = "6.0.0";
        status["engines_total"] = MAX_ENGINES;
        
        int healthy_count = 0;
        Json::Value engine_list(Json::arrayValue);
        for (auto& e : engines) {
            Json::Value node;
            node["name"] = e->name;
            node["scheme"] = e->scheme;
            node["healthy"] = e->healthy;
            node["requests"] = e->requests_handled.load();
            node["load_factor"] = e->load_factor;
            node["health_score"] = e->health_score;
            engine_list.append(node);
            if (e->healthy) healthy_count++;
        }
        status["engines_healthy"] = healthy_count;
        status["engines"] = engine_list;
        
        auto now = std::chrono::steady_clock::now();
        double uptime = std::chrono::duration<double>(now - start_time).count();
        status["uptime_seconds"] = uptime;
        status["total_requests"] = total_requests.load();
        
        return status;
    }
    
    Json::Value route_request(const std::string& operation, const Json::Value& params) {
        Json::Value response;
        total_requests++;
        
        int engine_idx = select_engine();
        auto& engine = engines[engine_idx];
        engine->requests_handled++;
        engine->load_factor = std::min(1.0, engine->load_factor + 0.1);
        
        response["engine"] = engine->name;
        response["scheme"] = engine->scheme;
        response["operation"] = operation;
        response["status"] = "ROUTED";
        
        double noise = 140.0;
        for (int i = 0; i < 10; i++) {
            noise = noise * PHI_INV + 40.0 * (1.0 - PHI_INV);
        }
        response["noise_level"] = noise;
        
        engine->load_factor = std::max(0.0, engine->load_factor - 0.05);
        return response;
    }
    
    void update_health_checks() {
        for (auto& engine : engines) {
            double error_rate = engine->requests_handled > 0 ? 
                (double)engine->errors / engine->requests_handled : 0.0;
            engine->health_score = calculate_health_score(error_rate, engine->load_factor);
            engine->healthy = engine->health_score > 0.3;
        }
    }
};

} // namespace hydra_gateway
#endif
