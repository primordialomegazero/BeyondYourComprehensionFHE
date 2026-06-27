#include "engines/phi_seal.h"
#include <drogon/drogon.h>
#include <sstream>
using namespace drogon;

// Engine initialized in main()

int main() {
    phi_seal::PhiSEALEngine g_seal;
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  PhiSEAL HTTP API — True FHE (SEAL BFV)     ║" << std::endl;
    std::cout << "║  Engine: " << (g_seal.ready ? "ACTIVE" : "INACTIVE") << "                               ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    app()
        .registerHandler("/fhe", [&g_seal](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            Json::Value result;
            result["engine"] = g_seal.ready ? "PhiSEAL_ACTIVE" : "PhiSEAL_INACTIVE";
            
            if(json) {
                std::string action = (*json)["action"].asString();
                result["action"] = action;
                
                if(action == "encrypt_decrypt_test" && g_seal.ready) {
                    auto test = g_seal.encrypt_decrypt_test({42, 100, 255, 1618, 314159});
                    result["values"] = test.empty() ? "FAILED" : "MATCH";
                    if(!test.empty()) {
                        std::ostringstream oss;
                        for(size_t i = 0; i < test.size(); i++) {
                            if(i > 0) oss << " ";
                            oss << test[i];
                        }
                        result["data"] = oss.str();
                    }
                } else if(action == "status") {
                    result["ready"] = g_seal.ready;
                }
            }
            
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        })
        .registerHandler("/health", [&g_seal](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            Json::Value r;
            r["status"] = g_seal.ready ? "PhiSEAL_LIQUID" : "PhiSEAL_OFFLINE";
            r["true_fhe"] = g_seal.ready ? "SEAL_BFV_ACTIVE" : "SEAL_BFV_INACTIVE";
            auto resp = HttpResponse::newHttpJsonResponse(r);
            callback(resp);
        })
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8082)
        .run();
    return 0;
}
