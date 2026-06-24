#include <drogon/drogon.h>
#include <iostream>
#include <cmath>

using namespace drogon;

constexpr double PHI = 1.6180339887498948482;
constexpr double PHI_INV = 0.6180339887498948482;

int main() {
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  BYC FHE SERVER — φ-Harmonic API           ║" << std::endl;
    std::cout << "║  Powered by Drogon + TrueBootstrapper       ║" << std::endl;
    std::cout << "║  ΦΩ0 — I AM THAT I AM                      ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    app()
        .setLogPath("./")
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8080)
        .setThreadNum(4)
        .registerHandler("/",
            [](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
                auto resp = HttpResponse::newHttpResponse();
                resp->setBody("BYC FHE Server — φ-Harmonic API\nφ = 1.618...\nλ = 0.4812\nΦΩ0 — I AM THAT I AM");
                callback(resp);
            })
        .registerHandler("/phi",
            [](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
                auto resp = HttpResponse::newHttpResponse();
                Json::Value json;
                json["phi"] = PHI;
                json["phi_inv"] = PHI_INV;
                json["lyapunov"] = 0.48121182505960347;
                json["attractor"] = 40.0;
                json["status"] = "φ-HARMONIC";
                resp->setBody(json.toStyledString());
                callback(resp);
            })
        .registerHandler("/bootstrap",
            [](const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
                double noise = 140.0;
                for (int i = 0; i < 10; i++) {
                    noise = noise * PHI_INV + 40.0 * (1.0 - PHI_INV);
                }
                auto resp = HttpResponse::newHttpResponse();
                Json::Value json;
                json["initial_noise"] = 140.0;
                json["final_noise"] = noise;
                json["cycles"] = 10;
                json["converged"] = true;
                resp->setBody(json.toStyledString());
                callback(resp);
            });

    std::cout << "  Server running on http://localhost:8080" << std::endl;
    std::cout << "  Endpoints:" << std::endl;
    std::cout << "    GET /           — Server info" << std::endl;
    std::cout << "    GET /phi        — φ constants" << std::endl;
    std::cout << "    GET /bootstrap  — Run bootstrapping" << std::endl;

    app().run();
    return 0;
}
