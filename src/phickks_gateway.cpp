#include "engines/phi_ckks.h"
#include <drogon/drogon.h>
#include <sstream>
using namespace drogon;

int main() {
    phi_seal::PhiCKKS g_ckks;
    
    std::cout << "╔══════════════════════════════════════════════╗" << std::endl;
    std::cout << "║  PhiCKKS HTTP API — Full Decimal FHE (CKKS) ║" << std::endl;
    std::cout << "║  Engine: " << (g_ckks.ready ? "ACTIVE" : "INACTIVE") << "                               ║" << std::endl;
    std::cout << "║  Slots: " << (g_ckks.ready ? g_ckks.slot_count() : 0) << "                             ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════╝" << std::endl;

    app()
        .registerHandler("/ckks", [&g_ckks](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            auto json = req->getJsonObject();
            Json::Value result;
            result["engine"] = g_ckks.ready ? "PhiCKKS_ACTIVE" : "PhiCKKS_INACTIVE";
            
            if(json) {
                std::string action = (*json)["action"].asString();
                result["action"] = action;
                
                if(action == "encrypt" && g_ckks.ready) {
                    double value = atof((*json)["value"].asString().c_str());
                    std::string ct = g_ckks.encrypt_decimal(value);
                    result["ciphertext"] = ct;
                    result["type"] = "decimal_ckks";
                    result["ciphertext_bytes"] = (int)ct.length() / 2;
                }
                else if(action == "decrypt" && g_ckks.ready) {
                    std::string ct = (*json)["ciphertext"].asString();
                    double pt = g_ckks.decrypt_decimal(ct);
                    result["plaintext"] = pt;
                    result["type"] = "decimal_ckks";
                }
                else if(action == "batch_encrypt" && g_ckks.ready) {
                    std::string vals_str = (*json)["values"].asString();
                    std::vector<double> vals;
                    std::stringstream vss(vals_str);
                    std::string token;
                    while(std::getline(vss, token, ',')) vals.push_back(atof(token.c_str()));
                    std::string ct = g_ckks.encrypt_batch(vals);
                    result["ciphertext"] = ct;
                    result["batch_size"] = (int)vals.size();
                    result["ciphertext_bytes"] = (int)ct.length() / 2;
                }
                else if(action == "batch_decrypt" && g_ckks.ready) {
                    std::string ct = (*json)["ciphertext"].asString();
                    auto vals = g_ckks.decrypt_batch(ct);
                    std::ostringstream oss;
                    for(size_t i = 0; i < vals.size() && i < 10; i++) {
                        if(i > 0) oss << ",";
                        oss << vals[i];
                    }
                    if(vals.size() > 10) oss << ",...(" << vals.size() << " total)";
                    result["plaintext"] = oss.str();
                    result["batch_size"] = (int)vals.size();
                }
                else if(action == "status") {
                    result["slots"] = g_ckks.ready ? g_ckks.slot_count() : 0;
                    result["scheme"] = "CKKS";
                    result["supports"] = "16,384 decimal slots, approximate arithmetic";
                }
            }
            
            auto resp = HttpResponse::newHttpJsonResponse(result);
            callback(resp);
        })
        .registerHandler("/health", [&g_ckks](const HttpRequestPtr& req,
            std::function<void(const HttpResponsePtr&)>&& callback) {
            Json::Value r;
            r["status"] = g_ckks.ready ? "PhiCKKS_LIQUID" : "PhiCKKS_OFFLINE";
            r["scheme"] = "CKKS";
            r["slots"] = g_ckks.ready ? g_ckks.slot_count() : 0;
            r["use_case"] = "ML inference, decimal arithmetic, statistical analysis";
            auto resp = HttpResponse::newHttpJsonResponse(r);
            callback(resp);
        })
        .setLogLevel(trantor::Logger::kWarn)
        .addListener("0.0.0.0", 8083)
        .run();
    return 0;
}
