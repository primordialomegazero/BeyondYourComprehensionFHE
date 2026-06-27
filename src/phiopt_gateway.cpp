#include "engines/phi_bfv_optimized.h"
#include <drogon/drogon.h>
#include <sstream>
#include <chrono>
using namespace drogon;

int main() {
    phi_seal::PhiBFVOptimized g_opt;
    
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  PhiBFV OPTIMIZED — 2048° φ-BFV FHE         ║" << std::endl;
    std::cout << "║  Engine: " << (g_opt.ready ? "ACTIVE" : "INACTIVE") << "                               ║" << std::endl;
    std::cout << "║  Slots: " << (g_opt.ready ? g_opt.slot_count() : 0) << "                              ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    app()
        .registerHandler("/opt", [&g_opt](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            Json::Value result;
            result["engine"] = g_opt.ready ? "PhiBFV_OPT_ACTIVE" : "PhiBFV_OPT_INACTIVE";
            result["slots"] = g_opt.ready ? g_opt.slot_count() : 0;
            
            if(json && g_opt.ready) {
                std::string action = (*json)["action"].asString();
                result["action"] = action;
                
                if(action == "encrypt") {
                    int64_t v = atoll((*json)["value"].asString().c_str());
                    auto start = std::chrono::steady_clock::now();
                    std::string ct = g_opt.encrypt_value(v);
                    auto end = std::chrono::steady_clock::now();
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    result["ciphertext"] = ct;
                    result["ct_bytes"] = (int)ct.length() / 2;
                    result["time_ms"] = (int)ms;
                }
                else if(action == "decrypt") {
                    std::string ct = (*json)["ciphertext"].asString();
                    auto start = std::chrono::steady_clock::now();
                    int64_t pt = g_opt.decrypt_value(ct);
                    auto end = std::chrono::steady_clock::now();
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    result["plaintext"] = std::to_string(pt);
                    result["time_ms"] = (int)ms;
                }
                else if(action == "add") {
                    std::string a = (*json)["ciphertext_a"].asString();
                    std::string b = (*json)["ciphertext_b"].asString();
                    auto start = std::chrono::steady_clock::now();
                    std::string sum = g_opt.add_ct(a, b);
                    auto end = std::chrono::steady_clock::now();
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    result["ciphertext"] = sum;
                    result["ct_bytes"] = (int)sum.length() / 2;
                    result["time_ms"] = (int)ms;
                    result["homomorphic"] = true;
                }
            }
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        })
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8084)
        .run();
    return 0;
}
