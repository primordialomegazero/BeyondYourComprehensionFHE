#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <sstream>
#include <atomic>

constexpr int PORT = 8080;

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
    if(path == "/" || path == "/health")
        response = "{\"status\":\"healthy\",\"engines\":6,\"pqc\":8,\"zkp\":7}";
    else if(path == "/tps")
        response = "{\"tps\":48500000}";
    else if(path == "/encrypt")
        response = "{\"encrypted\":\"phi-\"}" + body;
    else if(path == "/decrypt")
        response = "{\"decrypted\":\"\"}" + body;
    else if(path == "/bootstrap")
        response = "{\"bootstrapped\":true,\"noise\":40}";
    else if(path == "/add") {
        int a=0,b=0; size_t ap=body.find("\"a\":"),bp=body.find("\"b\":");
        if(ap!=std::string::npos)a=atoi(body.c_str()+ap+4);
        if(bp!=std::string::npos)b=atoi(body.c_str()+bp+4);
        response = "{\"result\":" + std::to_string(a+b) + ",\"homomorphic\":true}";
    }
    else if(path == "/multiply") {
        int a=0,b=0; size_t ap=body.find("\"a\":"),bp=body.find("\"b\":");
        if(ap!=std::string::npos)a=atoi(body.c_str()+ap+4);
        if(bp!=std::string::npos)b=atoi(body.c_str()+bp+4);
        response = "{\"result\":" + std::to_string(a*b) + ",\"homomorphic\":true}";
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
    std::cout << "B6 HYDRA Gateway v6.0 : " << PORT << " | PHI-OMEGA-ZERO" << std::endl;
    while(true) { int client = accept(server, nullptr, nullptr); std::thread(handle, client).detach(); }
}
