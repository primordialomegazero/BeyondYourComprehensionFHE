// Minimal SEAL API wrapper — proof that PhiSEAL works in HTTP
#include "engines/phi_seal.h"
#include <drogon/drogon.h>
using namespace drogon;

int main() {
    phi_seal::PhiSEALEngine seal;
    std::cout << "PhiSEAL HTTP API — Engine " << (seal.ready ? "ACTIVE" : "INACTIVE") << std::endl;

    app()
        .registerHandler("/seal", [&seal](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            Json::Value r;
            r["ready"] = seal.ready;
            if(seal.ready) {
                auto result = seal.encrypt_decrypt_test({42, 100, 255});
                r["test"] = result.empty() ? "FAILED" : "MATCH";
                r["values"] = result.empty() ? "[]" : std::to_string(result[0]) + "," + std::to_string(result[1]) + "," + std::to_string(result[2]);
            }
            auto resp = HttpResponse::newHttpJsonResponse(r);
            callback(resp);
        })
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8081)
        .run();
    return 0;
}
