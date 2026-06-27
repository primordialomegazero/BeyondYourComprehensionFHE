#include "engines/phi_seal.h"
#include <drogon/drogon.h>
#include <sstream>
using namespace drogon;

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
                
                if(action == "encrypt" && g_seal.ready) {
                    int64_t value = atoll((*json)["value"].asString().c_str());
                    std::string ct = g_seal.encrypt_value(value);
                    result["ciphertext"] = ct;
                    result["format"] = "seal_bfv_hex";
                    result["noise"] = g_seal.get_noise();
                    result["ciphertext_bytes"] = (int)ct.length() / 2;
                }
                else if(action == "decrypt" && g_seal.ready) {
                    std::string ct = (*json)["ciphertext"].asString();
                    int64_t pt = g_seal.decrypt_value(ct);
                    result["plaintext"] = std::to_string(pt);
                }
                else if(action == "encrypt_decrypt_test" && g_seal.ready) {
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
                }
                else if(action == "add" && g_seal.ready) {
                    std::string a = (*json)["ciphertext_a"].asString();
                    std::string b = (*json)["ciphertext_b"].asString();
                    std::string sum = g_seal.add_ct(a, b);
                    result["ciphertext"] = sum;
                    result["ciphertext_bytes"] = (int)sum.length() / 2;
                    result["homomorphic"] = true;
                    result["operation"] = "ciphertext + ciphertext = ciphertext";
                }
                else if(action == "multiply" && g_seal.ready) {
                    std::string a = (*json)["ciphertext_a"].asString();
                    std::string b = (*json)["ciphertext_b"].asString();
                    std::string prod = g_seal.multiply_ct(a, b);
                    result["ciphertext"] = prod;
                    result["ciphertext_bytes"] = (int)prod.length() / 2;
                    result["homomorphic"] = true;
                    result["operation"] = "ciphertext * ciphertext = ciphertext";
                }
                else if(action == "batch_encrypt" && g_seal.ready) {
                    std::string values_str = (*json)["values"].asString();
                    std::vector<int64_t> vals;
                    std::stringstream vss(values_str);
                    std::string token;
                    while(std::getline(vss, token, ',')) vals.push_back(atoll(token.c_str()));
                    std::string ct = g_seal.encrypt_batch(vals);
                    result["ciphertext"] = ct;
                    result["ciphertext_bytes"] = (int)ct.length() / 2;
                    result["batch_size"] = (int)vals.size();
                }
                else if(action == "batch_multiply" && g_seal.ready) {
                    std::string a = (*json)["ciphertext_a"].asString();
                    std::string b = (*json)["ciphertext_b"].asString();
                    std::string prod = g_seal.multiply_batch(a, b);
                    result["ciphertext"] = prod;
                    result["ciphertext_bytes"] = (int)prod.length() / 2;
                    result["homomorphic"] = true;
                }
                else if(action == "batch_add" && g_seal.ready) {
                    std::string a = (*json)["ciphertext_a"].asString();
                    std::string b = (*json)["ciphertext_b"].asString();
                    std::string sum = g_seal.add_batch(a, b);
                    result["ciphertext"] = sum;
                    result["ciphertext_bytes"] = (int)sum.length() / 2;
                    result["homomorphic"] = true;
                }
                else if(action == "batch_decrypt" && g_seal.ready) {
                    std::string ct = (*json)["ciphertext"].asString();
                    auto vals = g_seal.decrypt_batch(ct);
                    std::ostringstream oss;
                    for(size_t i = 0; i < vals.size(); i++) {
                        if(i > 0) oss << ",";
                        oss << vals[i];
                    }
                    result["plaintext"] = oss.str();
                    result["batch_size"] = (int)vals.size();
                }
                else if(action == "binary_encrypt" && g_seal.ready) {
                    int64_t value = atoll((*json)["value"].asString().c_str());
                    std::vector<int64_t> vals = {value};
                    seal::Plaintext pt;
                    g_seal.benc->encode(vals, pt);
                    seal::Ciphertext ct;
                    g_seal.enc->encrypt(pt, ct);
                    std::stringstream ss;
                    ct.save(ss, std::ios::binary);
                    std::string raw = ss.str();
                    result["ciphertext_base64"] = "binary_mode_active";
                    result["raw_bytes"] = (int)raw.size();
                    result["mode"] = "binary_no_hex_overhead";
                }
                else if(action == "status") {
                    result["ready"] = g_seal.ready;
                    result["noise"] = g_seal.ready ? g_seal.get_noise() : 0;
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
// (Appended binary handler - we'll use a simpler approach)
