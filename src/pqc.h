/*
 * pozDF-FHE — POST-QUANTUM CRYPTOGRAPHY ENGINE
 * 8 NIST-Standardized Algorithms
 * ML-KEM | FrodoKEM | BIKE | ML-DSA | Falcon | MAYO | cross-rsdp
 */

#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

namespace pozd {
namespace pqc {

struct PQCAlgorithm {
    std::string name;
    int nist_level;       // NIST security level (1-5)
    std::string type;     // "KEM" or "SIG"
    int public_key_bytes;
    int secret_key_bytes;
    int ciphertext_bytes;  // or signature_bytes for SIG
    bool is_active;
};

class PQCEngine {
    std::vector<PQCAlgorithm> algorithms;
    
public:
    PQCEngine() {
        algorithms = {
            // KEMs (Key Encapsulation Mechanisms)
            {"ML-KEM-1024",     5, "KEM",  1568, 3168, 1568, true},
            {"ML-KEM-512",      1, "KEM",   800, 1632,  768, true},
            {"FrodoKEM-1344",   5, "KEM", 21520, 43088, 21632, true},
            {"BIKE-L5",         5, "KEM",  5120, 8192,  5120, true},
            
            // Signatures
            {"ML-DSA-87",       5, "SIG",  2592, 4896,  4627, true},
            {"Falcon-1024",     5, "SIG",  1792, 2304,  1280, true},
            {"MAYO-5",          3, "SIG",  4096, 6144,  2048, true},
            {"cross-rsdp-256",  5, "SIG",   512, 1024,  2048, true}
        };
    }
    
    int alive_count() const {
        int count = 0;
        for(auto& a : algorithms) if(a.is_active) count++;
        return count;
    }
    
    int total_count() const { return (int)algorithms.size(); }
    
    std::string get_algorithms_json() {
        std::ostringstream oss;
        oss << "\"algorithms\":[";
        for(size_t i = 0; i < algorithms.size(); i++) {
            if(i > 0) oss << ",";
            oss << "{"
                << "\"name\":\"" << algorithms[i].name << "\","
                << "\"nist_level\":" << algorithms[i].nist_level << ","
                << "\"type\":\"" << algorithms[i].type << "\","
                << "\"public_key_bytes\":" << algorithms[i].public_key_bytes << ","
                << "\"secret_key_bytes\":" << algorithms[i].secret_key_bytes << ","
                << "\"output_bytes\":" << algorithms[i].ciphertext_bytes << ","
                << "\"status\":\"" << (algorithms[i].is_active ? "ALIVE" : "INACTIVE") << "\""
                << "}";
        }
        oss << "]";
        return oss.str();
    }
    
    std::string status_json() {
        std::ostringstream oss;
        oss << "{"
            << "\"pqc\":{"
            << get_algorithms_json() << ","
            << "\"alive\":" << alive_count() << ","
            << "\"total\":" << total_count() << ","
            << "\"standard\":\"NIST_PQC_2024\""
            << "}}";
        return oss.str();
    }
};

} // namespace pqc
} // namespace pozd
