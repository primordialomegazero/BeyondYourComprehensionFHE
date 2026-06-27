#include "engines/phi_seal.h"
#include "engines/phi_bfv_optimized.h"
#include "engines/phi_auto_harmonize.h"
#include <drogon/drogon.h>
#include <sstream>
#include <chrono>
using namespace drogon;

int main() {
    phi_seal::PhiSEALEngine g_seal;
    phi_seal::PhiBFVOptimized g_opt;
    
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  B6 HYDRA — AUTO-HARMONIZED UNIFIED FHE     ║" << std::endl;
    std::cout << "║  φ-Stream: ACTIVE | BFV: ACTIVE              ║" << std::endl;
    std::cout << "║  Mode: AUTO-HARMONIZE (φ-analysis)          ║" << std::endl;
    std::cout << "║  ΦΩ0 — I AM THAT I AM                       ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    app()
        .registerHandler("/manifest", [&g_seal, &g_opt](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            Json::Value result;
            
            if(!json) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setBody("{\"error\":\"Send JSON with 'action' field\"}");
                callback(resp);
                return;
            }

            std::string action = (*json)["action"].asString();
            std::string value = (*json).get("value", "").asString();
            std::string mode = (*json).get("mode", "auto").asString();
            
            result["action"] = action;
            result["phi"] = phi_seal::PHI;

            try {
                // ═══════════════════════════════════════
                // AUTO-HARMONIZATION: φ-analysis of data
                // ═══════════════════════════════════════
                phi_seal::AutoHarmonizer::HarmonyAnalysis harmony;
                if(!value.empty()) {
                    harmony = phi_seal::AutoHarmonizer::analyze(value);
                    result["harmony"] = Json::Value();
                    result["harmony"]["entropy"] = harmony.entropy;
                    result["harmony"]["phi_resonance"] = harmony.phi_resonance;
                    result["harmony"]["data_size"] = (int)harmony.data_size;
                    result["harmony"]["has_decimals"] = harmony.has_decimals;
                    result["harmony"]["recommended"] = phi_seal::AutoHarmonizer::mode_name(harmony.recommended_mode);
                    
                    // Override mode if auto
                    if(mode == "auto") {
                        switch(harmony.recommended_mode) {
                            case phi_seal::AutoHarmonizer::Mode::STREAM: mode = "quick"; break;
                            case phi_seal::AutoHarmonizer::Mode::BFV_2048: mode = "fhe"; break;
                            case phi_seal::AutoHarmonizer::Mode::BFV_4096: mode = "fhe"; break;
                            case phi_seal::AutoHarmonizer::Mode::CKKS: mode = "ckks"; break;
                        }
                    }
                }
                result["active_mode"] = mode;
                
                // ═══════════════════════════════════════
                // EXECUTE based on harmonized mode
                // ═══════════════════════════════════════
                if(action == "encrypt") {
                    if(mode == "quick") {
                        // φ-Stream cipher
                        std::string ct;
                        double n = 140.0;
                        for(char c : value) {
                            n = n * phi_seal::PHI_INV + 40.0 * (1.0 - phi_seal::PHI_INV);
                            ct += (char)(c ^ (uint8_t)(std::abs(std::sin(n * phi_seal::PHI)) * 255.0));
                        }
                        std::ostringstream oss;
                        for(unsigned char c : ct) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
                        result["ciphertext"] = oss.str();
                        result["ct_bytes"] = (int)ct.size();
                    }
                    else if(mode == "fhe" && g_opt.ready) {
                        auto start = std::chrono::steady_clock::now();
                        int64_t v = atoll(value.c_str());
                        std::string ct = g_opt.encrypt_value(v);
                        auto end = std::chrono::steady_clock::now();
                        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                        result["ciphertext"] = ct;
                        result["ct_bytes"] = (int)ct.length() / 2;
                        result["time_ms"] = (int)ms;
                    }
                }
                else if(action == "decrypt" && g_opt.ready) {
                    std::string ct = (*json)["ciphertext"].asString();
                    int64_t pt = g_opt.decrypt_value(ct);
                    result["plaintext"] = std::to_string(pt);
                }
                else if(action == "add" && g_opt.ready) {
                    std::string a = (*json)["ciphertext_a"].asString();
                    std::string b = (*json)["ciphertext_b"].asString();
                    auto start = std::chrono::steady_clock::now();
                    std::string sum = g_opt.add_ct(a, b);
                    auto end = std::chrono::steady_clock::now();
                    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    result["ciphertext"] = sum;
                    result["time_ms"] = (int)ms;
                    result["homomorphic"] = true;
                }
                else {
                    result["error"] = "Unknown action";
                }
            } catch(...) {
                result["error"] = "Operation failed";
            }

            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        })
        .registerHandler("/health", [&g_seal, &g_opt](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            Json::Value r;
            r["status"] = "LIQUID_AUTO_HARMONIZED";
            r["architecture"] = "AUTO_HARMONIZED_UNIFIED";
            r["phi"] = phi_seal::PHI;
            r["schumann"] = phi_seal::SCHUMANN;
            r["engines"] = "φ-Stream + BFV 4096° + BFV 2048°";
            r["mode"] = "AUTO (φ-analysis of data entropy & resonance)";
            auto resp = HttpResponse::newHttpJsonResponse(r);
            callback(resp);
        })
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8085)
        .run();
    return 0;
}
