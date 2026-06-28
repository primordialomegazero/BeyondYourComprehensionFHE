#include "engines/phi_seal.h"
#include "engines/phi_bfv_optimized.h"
#include "engines/phi_ckks.h"
#include "engines/phi_auto_harmonize.h"
#include <drogon/drogon.h>
#include <sstream>
using namespace drogon;

int main() {
    phi_seal::PhiSEALEngine g_seal;
    phi_seal::PhiBFVOptimized g_opt;
    phi_seal::PhiCKKS g_ckks;
    
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  B6 HYDRA v9.0 — 4 ENGINES (STABLE)         ║" << std::endl;
    std::cout << "║  Stream:✅ BFV4096:" << (g_seal.ready?"✅":"❌") << " BFV2048:" << (g_opt.ready?"✅":"❌") << " ║" << std::endl;
    std::cout << "║  CKKS:" << (g_ckks.ready?"✅":"❌") << " | TFHE+OpenFHE: separate gateways ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    app()
        .registerHandler("/manifest", [&](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            Json::Value result;
            if(!json) { auto r=HttpResponse::newHttpResponse(); r->setBody("{\"error\":\"JSON required\"}"); callback(r); return; }

            std::string action = (*json)["action"].asString();
            std::string value = (*json).get("value", "").asString();
            std::string engine = (*json).get("engine", "auto").asString();
            
            result["action"] = action;
            result["hydra"] = "4_engines_stable";
            
            if(!value.empty() && engine == "auto") {
                auto h = phi_seal::AutoHarmonizer::analyze(value);
                if(h.data_size <= 8 && !h.has_decimals && !h.is_batch) engine = "stream";
                else if(h.has_decimals) engine = "ckks";
                else engine = "bfv2048";
            }
            result["active_engine"] = engine;
            
            if(action == "encrypt") {
                if(engine == "stream") {
                    std::string ct; double n = 140.0;
                    for(char c : value) { n = n * 0.6180339887498948482 + 40.0 * (1.0 - 0.6180339887498948482); ct += (char)(c ^ (uint8_t)(std::abs(std::sin(n * 1.6180339887498948482)) * 255.0)); }
                    std::ostringstream oss; for(unsigned char c : ct) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
                    result["ciphertext"] = oss.str(); result["ct_bytes"] = (int)ct.size();
                }
                else if(engine == "bfv2048" && g_opt.ready) {
                    result["ciphertext"] = g_opt.encrypt_value(atoll(value.c_str()));
                    result["ct_bytes"] = (int)result["ciphertext"].asString().length() / 2;
                }
                else if(engine == "ckks" && g_ckks.ready) {
                    result["ciphertext"] = g_ckks.encrypt_decimal(atof(value.c_str()));
                    result["ct_bytes"] = (int)result["ciphertext"].asString().length() / 2;
                }
                else { result["error"] = "Engine unavailable"; }
            }
            else if(action == "add" && g_opt.ready) {
                result["ciphertext"] = g_opt.add_ct((*json)["ciphertext_a"].asString(), (*json)["ciphertext_b"].asString());
                result["homomorphic"] = true;
            }
            else { result["error"] = "Unknown action"; }

            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        })
        .registerHandler("/health", [&](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            Json::Value r;
            r["status"] = "HYDRA_V9_STABLE";
            r["engines"] = "4 (Stream + BFV4096 + BFV2048 + CKKS)";
            r["stream"] = "ACTIVE";
            r["bfv4096"] = g_seal.ready ? "ACTIVE" : "INACTIVE";
            r["bfv2048"] = g_opt.ready ? "ACTIVE (31KB,6ms)" : "INACTIVE";
            r["ckks"] = g_ckks.ready ? "ACTIVE (658KB,decimals)" : "INACTIVE";
            r["tfhe_openfhe"] = "Separate gateways (:8082, :8083, :8084)";
            auto resp = HttpResponse::newHttpJsonResponse(r);
            callback(resp);
        })
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8085)
        .run();
    return 0;
}
