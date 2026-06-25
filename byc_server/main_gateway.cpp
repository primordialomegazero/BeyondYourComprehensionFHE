#include <drogon/drogon.h>
#include <iostream>
#include <json/json.h>
#include "phi_gateway.h"

using namespace drogon;

int main() {
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  Φ-PLANETARY CONSCIOUSNESS GATEWAY          ║" << std::endl;
    std::cout << "║  Source: 12.67 Hz | Schumann: 7.83 Hz       ║" << std::endl;
    std::cout << "║  ΦΩ0 — I AM THAT I AM                      ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    app()
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8080)
        .setThreadNum(12);

    // MAIN GATEWAY ENDPOINT
    app().registerHandler("/gateway",
        [](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpResponse();
            Json::Value request;
            
            if (req->method() == Post) {
                auto body = req->getJsonObject();
                if (body) {
                    request = *body;
                }
            }
            
            Json::Value result = phi_gateway::PhiGateway::process_request(request);
            resp->setBody(result.toStyledString());
            callback(resp);
        });

    // QUICK ENDPOINTS
    app().registerHandler("/",
        [](const HttpRequestPtr &req,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newHttpResponse();
            Json::Value json;
            json["gateway"] = "Φ-Planetary Consciousness API";
            json["source"] = "12.67 Hz (ΦΩ0)";
            json["schumann"] = "7.83 Hz (Earth)";
            json["endpoints"] = "/gateway, /health, /phi";
            resp->setBody(json.toStyledString());
            callback(resp);
        });

    std::cout << "    Gateway running on http://localhost:8080" << std::endl;
    std::cout << "  🌍 POST /gateway — Process through Source" << std::endl;

    app().run();
    return 0;
}
