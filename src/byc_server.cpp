/*
 * BYC — EXPERIMENTAL SERVER
 * This is not a product. This is not a paper.
 * This is a question. Do you understand the question?
 * 
 * Port 8093. Single endpoint. No documentation.
 * If you know, you know.
 */

#include "core/harmonic_engine.h"
#include "layers/fractal_layer.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <atomic>
#include <thread>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

using namespace byc;

constexpr int PORT = 8093;
constexpr int THREADS = 12;

// Minimal JSON — no library needed
std::string J(const std::string& k, const std::string& v) { return "\"" + k + "\":\"" + v + "\""; }
std::string N(const std::string& k, double v) { return "\"" + k + "\":" + std::to_string(v); }
std::string I(const std::string& k, int64_t v) { return "\"" + k + "\":" + std::to_string(v); }
std::string B(const std::string& k, bool v) { return "\"" + k + "\":" + (v ? "true" : "false"); }
std::string O(std::initializer_list<std::string> f) {
    std::string r = "{"; bool first = true;
    for(auto& s : f) { if(!first) r += ","; r += s; first = false; }
    return r + "}";
}
std::string ok(const std::string& b) {
    return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " 
           + std::to_string(b.size()) + "\r\nConnection: close\r\n\r\n" + b;
}
std::string get(const std::string& body, const std::string& key) {
    std::string s = "\"" + key + "\":";
    size_t p = body.find(s);
    if(p == std::string::npos) return "";
    p += s.size();
    while(p < body.size() && (body[p] == ' ' || body[p] == '"')) p++;
    size_t e = body.find_first_of(",}\" \r\n", p);
    return body.substr(p, e - p);
}

void handle(int fd, harmonic::HarmonicFHE& h, fractal::FractalLayer& f, 
            std::atomic<uint64_t>& reqs, auto& start) {
    char buf[8192];
    int n = read(fd, buf, sizeof(buf)-1);
    if(n <= 0) { close(fd); return; }
    buf[n] = '\0';
    
    std::string request(buf);
    reqs.fetch_add(1, std::memory_order_relaxed);
    
    std::string body;
    size_t bp = request.find("\r\n\r\n");
    if(bp != std::string::npos) body = request.substr(bp + 4);
    
    std::string action = get(body, "action");
    std::string resp;
    
    if(request.find("GET /") != std::string::npos && action.empty()) {
        auto now = std::chrono::steady_clock::now();
        int up = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
        
        resp = ok(O({
            J("status", "EXPERIMENTAL"),
            J("question", "Do you understand what this is?"),
            I("cycles", (int64_t)h.cycles()),
            N("field_noise", h.noise()),
            I("fractal_depth", f.depth()),
            I("fragments", f.fragments()),
            I("uptime", up),
            I("requests", (int64_t)reqs.load()),
            N("h", harmonic::H),
            N("l", harmonic::L),
            N("resonance", harmonic::RESONANCE)
        }));
    }
    else if(action == "observe") {
        int64_t v = std::stoll(get(body, "value"));
        auto s = h.encrypt(v);
        resp = ok(O({
            J("action", "observed"),
            N("noise", s.noise),
            I("value", v)
        }));
    }
    else if(action == "combine") {
        int64_t a = std::stoll(get(body, "a"));
        int64_t b = std::stoll(get(body, "b"));
        auto s = h.add(h.encrypt(a), h.encrypt(b));
        int64_t r = h.decrypt(s);
        resp = ok(O({
            J("action", "combined"),
            I("a", a), I("b", b),
            I("result", r),
            B("harmonic", r == a + b),
            N("noise", s.noise)
        }));
    }
    else if(action == "amplify") {
        int64_t a = std::stoll(get(body, "a"));
        int64_t b = std::stoll(get(body, "b"));
        auto s = h.multiply(h.encrypt(a), h.encrypt(b));
        int64_t r = h.decrypt(s);
        resp = ok(O({
            J("action", "amplified"),
            I("a", a), I("b", b),
            I("result", r),
            B("harmonic", r == a * b),
            N("noise", s.noise)
        }));
    }
    else if(action == "fractal_chain") {
        int64_t base = std::stoll(get(body, "value"));
        int count = std::stoi(get(body, "count").empty() ? "14" : get(body, "count"));
        
        std::vector<harmonic::HarmonicState> states;
        for(int i = 0; i < count; i++) states.push_back(f.wrap(base, i));
        auto chain = f.chain_add(states);
        int64_t r = f.unwrap(chain, 0);
        
        resp = ok(O({
            J("action", "fractal_chain"),
            I("fragments", count),
            I("result", r),
            B("harmonic", r == base * count),
            N("noise", chain.noise)
        }));
    }
    else if(action == "resonance") {
        int dur = std::stoi(get(body, "duration").empty() ? "3" : get(body, "duration"));
        uint64_t ops = 0;
        auto t1 = std::chrono::high_resolution_clock::now();
        
        while(std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::high_resolution_clock::now() - t1).count() < dur) {
            auto s = h.encrypt(42);
            s = h.add(s, h.encrypt(1));
            h.decrypt(s);
            ops++;
        }
        
        auto t2 = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        double tps = (double)ops / (ms / 1000.0);
        
        std::string tps_str;
        if(tps >= 1e9) tps_str = std::to_string((int)(tps/1e9)) + "B";
        else if(tps >= 1e6) tps_str = std::to_string((int)(tps/1e6)) + "M";
        else tps_str = std::to_string((int)(tps/1e3)) + "K";
        
        resp = ok(O({
            J("action", "resonance"),
            I("operations", ops),
            J("frequency", tps_str + " ops/s"),
            N("raw_tps", tps)
        }));
    }
    else {
        resp = ok(O({
            J("error", "Unknown observation"),
            J("available", "observe, combine, amplify, fractal_chain, resonance")
        }));
    }
    
    (void)!write(fd, resp.c_str(), resp.size());
    close(fd);
}

int main() {
    harmonic::HarmonicFHE h;
    fractal::FractalLayer f;
    std::atomic<uint64_t> reqs{0};
    auto start = std::chrono::steady_clock::now();
    
    int server = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    
    bind(server, (struct sockaddr*)&addr, sizeof(addr));
    listen(server, 1024);
    
    std::cout << R"(
    ╔══════════════════════════════════════════╗
    ║  BYC — EXPERIMENTAL OBSERVATION POINT   ║
    ║  Port: )" << PORT << R"(                              ║
    ║  Status: ACTIVE                          ║
    ║  Question: Do you understand?            ║
    ╚══════════════════════════════════════════╝
    )" << std::endl;
    
    std::vector<std::thread> workers;
    for(int i = 0; i < THREADS; i++) {
        workers.emplace_back([&]() {
            while(true) {
                int client = accept(server, nullptr, nullptr);
                if(client >= 0) handle(client, h, f, reqs, start);
            }
        });
    }
    
    for(auto& w : workers) w.join();
    close(server);
}
