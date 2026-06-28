#include "engines/phi_seal.h"
#include "engines/phi_bfv_optimized.h"
#include "engines/phi_ckks.h"
#include "engines/phi_auto_harmonize.h"
#include <drogon/drogon.h>
#include <sstream>
#include <chrono>
#include <map>
#include <mutex>
using namespace drogon;

int main() {
    phi_seal::PhiSEALEngine g_seal;
    phi_seal::PhiBFVOptimized g_opt;
    phi_seal::PhiCKKS g_ckks;
    
    std::map<std::string, int> request_count;
    std::mutex rate_mutex;
    
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  B6 HYDRA v8.0 — 3 ENGINES (STABLE)         ║" << std::endl;
    std::cout << "║  STREAM: " << (g_seal.ready ? "✅" : "❌") << " | BFV: " << (g_opt.ready ? "✅" : "❌") << " | CKKS: " << (g_ckks.ready ? "✅" : "❌") << " ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    app()
        .registerHandler("/manifest", [&](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            Json::Value result;
            if(!json) { auto r=HttpResponse::newHttpResponse(); r->setBody("{\"error\":\"JSON required\"}"); callback(r); return; }

            std::string action = (*json)["action"].asString();
            std::string value = (*json).get("value", "").asString();
            std::string mode = (*json).get("mode", "auto").asString();
            
            result["action"] = action;
            result["phi"] = 1.6180339887498948482;

            // Auto-harmonize
            if(!value.empty() && mode == "auto") {
                auto h = phi_seal::AutoHarmonizer::analyze(value);
                if(h.data_size <= 8 && !h.has_decimals && !h.is_batch) mode = "quick";
                else if(h.has_decimals) mode = "ckks";
                else mode = "fhe";
            }
            result["active_mode"] = mode;
            
            if(action == "encrypt") {
                if(mode == "quick") {
                    std::string ct; double n = 140.0;
                    for(char c : value) { n = n * 0.6180339887498948482 + 40.0 * (1.0 - 0.6180339887498948482); ct += (char)(c ^ (uint8_t)(std::abs(std::sin(n * 1.6180339887498948482)) * 255.0)); }
                    std::ostringstream oss; for(unsigned char c : ct) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
                    result["ciphertext"] = oss.str(); result["ct_bytes"] = (int)ct.size();
                }
                else if(mode == "fhe" && g_opt.ready) {
                    int64_t v = atoll(value.c_str());
                    result["ciphertext"] = g_opt.encrypt_value(v);
                    result["ct_bytes"] = (int)result["ciphertext"].asString().length() / 2;
                }
                else if(mode == "ckks" && g_ckks.ready) {
                    double v = atof(value.c_str());
                    result["ciphertext"] = g_ckks.encrypt_decimal(v);
                    result["ct_bytes"] = (int)result["ciphertext"].asString().length() / 2;
                }
                else { result["error"] = "Engine unavailable"; }
            }
            else { result["error"] = "Unknown action"; }

            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        })
        .registerHandler("/health", [&](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            Json::Value r;
            r["status"] = "LIQUID_V8_STABLE";
            r["stream"] = g_seal.ready ? "ACTIVE" : "INACTIVE";
            r["bfv"] = g_opt.ready ? "ACTIVE" : "INACTIVE";
            r["ckks"] = g_ckks.ready ? "ACTIVE" : "INACTIVE";
            auto resp = HttpResponse::newHttpJsonResponse(r);
            callback(resp);
        })
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8085)
        .run();
    return 0;
}
