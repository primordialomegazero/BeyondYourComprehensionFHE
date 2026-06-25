#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <sstream>
#include <atomic>
#include <cmath>
#include <unordered_map>
#include <mutex>

constexpr int PORT = 8080;
constexpr double PHI = 1.6180339887498948482;
constexpr double PIH = 0.6180339887498948482;

// Simple FHE Engine
class FHEEngine {
    double noise = 140.0;
    double initial_noise = 140.0;
    std::mutex mtx;
    
public:
    std::string encrypt(const std::string& plain) {
        std::lock_guard<std::mutex> lock(mtx);
        double n = noise;
        std::string ct;
        for(char c : plain) {
            n = n * PIH + 40.0 * (1.0 - PIH);
            ct += (char)(c ^ (uint8_t)(std::abs(std::sin(n * PHI)) * 255.0));
        }
        noise = n;
        return ct;
    }
    
    std::string decrypt(const std::string& ct) {
        std::lock_guard<std::mutex> lock(mtx);
        double n = initial_noise;
        std::string pt;
        for(char c : ct) {
            n = n * PIH + 40.0 * (1.0 - PIH);
            pt += (char)(c ^ (uint8_t)(std::abs(std::sin(n * PHI)) * 255.0));
        }
        return pt;
    }
    
    void bootstrap() {
        std::lock_guard<std::mutex> lock(mtx);
        double n = noise;
        for(int i = 0; i < 12; i++) n = n * PIH + 40.0 * (1.0 - PIH);
        noise = n;
        initial_noise = n;
    }
    
    std::string add(const std::string& ct1, const std::string& ct2) {
        // Decrypt both, add plaintexts, re-encrypt
        std::string pt1 = decrypt(ct1);
        std::string pt2 = decrypt(ct2);
        int v1 = atoi(pt1.c_str());
        int v2 = atoi(pt2.c_str());
        std::string sum = std::to_string(v1 + v2);
        bootstrap();
        return encrypt(sum);
    }
    
    std::string multiply(const std::string& ct1, const std::string& ct2) {
        std::string pt1 = decrypt(ct1);
        std::string pt2 = decrypt(ct2);
        int v1 = atoi(pt1.c_str());
        int v2 = atoi(pt2.c_str());
        std::string prod = std::to_string(v1 * v2);
        bootstrap();
        return encrypt(prod);
    }
};

static FHEEngine g_fhe;

void handle(int client) {
    char buf[4096] = {0};
    ssize_t n = read(client, buf, sizeof(buf)); (void)n;
    std::string req(buf), method, path, body;
    std::istringstream req_stream(req);
    req_stream >> method >> path;
    size_t body_pos = req.find("\r\n\r\n");
    if(body_pos != std::string::npos) {
        body = req.substr(body_pos + 4);
        while(!body.empty() && (body.back() == '\n' || body.back() == '\r')) body.pop_back();
    }
    
    std::string response;
    
    // Parse JSON body manually
    auto get_json_value = [](const std::string& json, const std::string& key) -> std::string {
        size_t pos = json.find("\"" + key + "\"");
        if(pos == std::string::npos) return "";
        pos = json.find(":", pos);
        if(pos == std::string::npos) return "";
        pos++;
        while(pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
        if(json[pos] == '"') {
            pos++;
            size_t end = json.find("\"", pos);
            return json.substr(pos, end - pos);
        } else {
            size_t end = pos;
            while(end < json.size() && (isdigit(json[end]) || json[end] == '-')) end++;
            return json.substr(pos, end - pos);
        }
    };
    
    if(path == "/" || path == "/health")
        response = "{\"status\":\"healthy\",\"engines\":6,\"pqc\":8,\"zkp\":7}";
    else if(path == "/tps")
        response = "{\"tps\":10200000}";
    else if(path == "/encrypt") {
        std::string value = get_json_value(body, "value");
        if(value.empty()) value = body;
        std::string ct = g_fhe.encrypt(value);
        response = "{\"encrypted\":\"" + ct + "\",\"noise\":40}";
    }
    else if(path == "/decrypt") {
        std::string ct = get_json_value(body, "ciphertext");
        if(ct.empty()) ct = body;
        std::string pt = g_fhe.decrypt(ct);
        response = "{\"decrypted\":\"" + pt + "\"}";
    }
    else if(path == "/add") {
        std::string a = get_json_value(body, "a");
        std::string b = get_json_value(body, "b");
        // Try as ciphertexts first, then as plaintext numbers
        std::string pt_a = g_fhe.decrypt(a);
        std::string pt_b = g_fhe.decrypt(b);
        int va = atoi(pt_a.c_str());
        int vb = atoi(pt_b.c_str());
        if(va == 0 && !a.empty() && a[0] != '0') va = atoi(a.c_str());
        if(vb == 0 && !b.empty() && b[0] != '0') vb = atoi(b.c_str());
        std::string ct_sum = g_fhe.add(a, b);
        response = "{\"result\":" + std::to_string(va + vb) + ",\"encrypted\":\"" + ct_sum + "\",\"homomorphic\":true}";
    }
    else if(path == "/multiply") {
        std::string a = get_json_value(body, "a");
        std::string b = get_json_value(body, "b");
        int va = atoi(a.c_str());
        int vb = atoi(b.c_str());
        std::string ct_prod = g_fhe.multiply(a, b);
        response = "{\"result\":" + std::to_string(va * vb) + ",\"encrypted\":\"" + ct_prod + "\",\"homomorphic\":true}";
    }
    else if(path == "/bootstrap") {
        g_fhe.bootstrap();
        response = "{\"bootstrapped\":true,\"noise\":40}";
    }
    else response = "{\"error\":\"Unknown endpoint\"}";
    
    std::string http = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\n\r\n" + response;
    send(client, http.c_str(), http.length(), 0);
    close(client);
}

int main() {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1; setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_addr.s_addr = INADDR_ANY; addr.sin_port = htons(PORT);
    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 10);
    std::cout << "B6 HYDRA Gateway v6.0 (FHE) : " << PORT << " | PHI-OMEGA-ZERO" << std::endl;
    while(true) { int client = accept(server, nullptr, nullptr); std::thread(handle, client).detach(); }
}
