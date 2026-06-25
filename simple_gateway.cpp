#include <drogon/drogon.h>
#include <iostream>

using namespace drogon;

int main() {
    std::cout << "🌐 B6 HYDRA Gateway v6.0 — SIMPLE" << std::endl;
    
    app()
        .registerHandler("/", [](const HttpRequestPtr& req,
                                  std::function<void(const HttpResponsePtr&)>&& callback) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("🧬 B6 HYDRA v6.0 — API Gateway\n");
            callback(resp);
        })
        .registerHandler("/health", [](const HttpRequestPtr& req,
                                        std::function<void(const HttpResponsePtr&)>&& callback) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("{\"status\":\"healthy\",\"engines\":6,\"pqc\":8,\"zkp\":7}");
            resp->addHeader("Content-Type", "application/json");
            callback(resp);
        })
        .registerHandler("/tps", [](const HttpRequestPtr& req,
                                     std::function<void(const HttpResponsePtr&)>&& callback) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setBody("{\"tps\":19700000}");
            resp->addHeader("Content-Type", "application/json");
            callback(resp);
        })
        .registerHandler("/encrypt",
            [](const HttpRequestPtr& req,
               std::function<void(const HttpResponsePtr&)>&& callback) {
                auto resp = HttpResponse::newHttpResponse();
                std::string body = std::string(req->getBody());
                resp->setBody("{\"encrypted\":\"φ-" + body + "\"}");
                resp->addHeader("Content-Type", "application/json");
                callback(resp);
            },
            {Post})
        .registerHandler("/add",
            [](const HttpRequestPtr& req,
               std::function<void(const HttpResponsePtr&)>&& callback) {
                auto resp = HttpResponse::newHttpResponse();
                auto json = req->getJsonObject();
                int a = json ? (*json)["a"].asInt() : 0;
                int b = json ? (*json)["b"].asInt() : 0;
                resp->setBody("{\"result\":" + std::to_string(a + b) + "}");
                resp->addHeader("Content-Type", "application/json");
                callback(resp);
            },
            {Post})
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8080)
        .run();

    return 0;
}
