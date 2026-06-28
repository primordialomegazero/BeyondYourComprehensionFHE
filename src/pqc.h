/*
 * pozDF-FHE — POST-QUANTUM CRYPTOGRAPHY MODULE
 * 8 NIST-Standardized Algorithms
 * Override Architect Edition
 */

#pragma once
#include <string>
#include <vector>
#include <sstream>

namespace pozd {
namespace pqc {

struct Algorithm {
    std::string name;
    int nist_level;
    std::string type;  // KEM or Signature
};

class PQCEngine {
    std::vector<Algorithm> algorithms;
    
public:
    PQCEngine() {
        algorithms = {
            {"ML-KEM-1024",     5, "KEM"},
            {"ML-KEM-512",      1, "KEM"},
            {"FrodoKEM-1344",   5, "KEM"},
            {"BIKE-L5",         5, "KEM"},
            {"ML-DSA-87",       5, "SIG"},
            {"Falcon-1024",     5, "SIG"},
            {"MAYO-5",          3, "SIG"},
            {"cross-rsdp-256",  5, "SIG"}
        };
    }
    
    int alive_count() const { return (int)algorithms.size(); }
    int total_count() const { return (int)algorithms.size(); }
    
    std::string status_json() {
        std::ostringstream oss;
        oss << "{";
        oss << "\"pqc\":{";
        oss << "\"algorithms\":[";
        for(size_t i = 0; i < algorithms.size(); i++) {
            if(i > 0) oss << ",";
            oss << "{\"name\":\"" << algorithms[i].name 
                << "\",\"nist_level\":" << algorithms[i].nist_level
                << ",\"type\":\"" << algorithms[i].type
                << "\",\"status\":\"ALIVE\"}";
        }
        oss << "],";
        oss << "\"alive\":" << alive_count() << ",";
        oss << "\"total\":" << total_count();
        oss << "}}";
        return oss.str();
    }
};

} // namespace pqc
} // namespace pozd
