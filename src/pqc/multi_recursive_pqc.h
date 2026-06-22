#pragma once

#include <oqs/oqs.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <map>
#include <memory>

class MultiRecursivePQC {
public:
    static constexpr double PHI = 1.6180339887498948482;
    static constexpr size_t MAX_RECURSION = 7;
    
    enum PQCOp { KEM_ENCAP, KEM_DECAP, SIG_SIGN, SIG_VERIFY };
    
    struct PQCNode {
        std::string algorithm;
        std::string type;
        int nist_level;
        size_t depth;
        std::vector<uint8_t> public_key;
        std::vector<uint8_t> secret_key;
        std::vector<uint8_t> signature_or_ciphertext;
        std::vector<uint8_t> shared_secret;
        bool alive;
        std::vector<PQCNode> children;
    };
    
    struct MultiPQCResult {
        std::vector<PQCNode> roots;
        size_t total_nodes;
        size_t max_depth;
        bool all_alive;
        std::map<std::string, bool> status;
    };
    
    static MultiPQCResult initialize_forest() {
        MultiPQCResult result;
        result.total_nodes = 0;
        result.max_depth = 0;
        result.all_alive = true;
        
        struct RootConfig { std::string name, type; int nist; };
        std::vector<RootConfig> roots = {
            {"ML-KEM-1024", "KEM", 5}, {"ML-KEM-512", "KEM", 1},
            {"FrodoKEM-1344-AES", "KEM", 5}, {"BIKE-L5", "KEM", 5},
            {"ML-DSA-87", "SIG", 5}, {"Falcon-1024", "SIG", 5},
            {"MAYO-5", "SIG", 3}, {"cross-rsdp-256-small", "SIG", 5}
        };
        
        for (const auto& rc : roots) {
            PQCNode root = create_node(rc.name, rc.type, rc.nist, 0);
            result.roots.push_back(root);
            result.total_nodes++;
            result.status[rc.name] = root.alive;
            if (!root.alive) result.all_alive = false;
        }
        return result;
    }
    
    static void spawn_children(PQCNode& parent, size_t max_depth, size_t branch_factor = 3) {
        if (parent.depth >= max_depth || !parent.alive) return;
        for (size_t b = 0; b < branch_factor; b++) {
            PQCNode child = create_node(parent.algorithm, parent.type, parent.nist_level, parent.depth + 1);
            if (child.alive) {
                spawn_children(child, max_depth, std::max((size_t)2, branch_factor - 1));
                parent.children.push_back(child);
            }
        }
    }
    
    static std::vector<uint8_t> recursive_sign_chain(PQCNode& node, const std::string& message, size_t depth = 0) {
        if (!node.alive || node.type != "SIG") return {};
        OQS_SIG* sig = OQS_SIG_new(node.algorithm.c_str());
        if (!sig) return {};
        std::vector<uint8_t> signature(sig->length_signature);
        size_t sig_len = signature.size();
        OQS_SIG_sign(sig, signature.data(), &sig_len, (const uint8_t*)message.c_str(), message.size(), node.secret_key.data());
        OQS_SIG_free(sig);
        node.signature_or_ciphertext = signature;
        for (auto& child : node.children) {
            recursive_sign_chain(child, message + ":" + std::to_string(depth) + ":" + std::to_string(child.depth), depth + 1);
        }
        return signature;
    }
    
    static bool verify_signature_tree(PQCNode& node, const std::string& message, size_t depth = 0) {
        if (!node.alive || node.type != "SIG") return true;
        OQS_SIG* sig = OQS_SIG_new(node.algorithm.c_str());
        if (!sig) return false;
        bool valid = (OQS_SIG_verify(sig, node.signature_or_ciphertext.data(), node.signature_or_ciphertext.size(), (const uint8_t*)message.c_str(), message.size(), node.public_key.data()) == OQS_SUCCESS);
        OQS_SIG_free(sig);
        if (!valid) return false;
        for (auto& child : node.children) {
            if (!verify_signature_tree(child, message + ":" + std::to_string(depth) + ":" + std::to_string(child.depth), depth + 1)) return false;
        }
        return true;
    }

private:
    static PQCNode create_node(const std::string& name, const std::string& type, int nist, size_t depth) {
        PQCNode node;
        node.algorithm = name; node.type = type; node.nist_level = nist; node.depth = depth; node.alive = false;
        if (type == "KEM") {
            OQS_KEM* kem = OQS_KEM_new(name.c_str());
            if (kem) {
                node.public_key.resize(kem->length_public_key);
                node.secret_key.resize(kem->length_secret_key);
                if (OQS_KEM_keypair(kem, node.public_key.data(), node.secret_key.data()) == OQS_SUCCESS) node.alive = true;
                OQS_KEM_free(kem);
            }
        } else {
            OQS_SIG* sig = OQS_SIG_new(name.c_str());
            if (sig) {
                node.public_key.resize(sig->length_public_key);
                node.secret_key.resize(sig->length_secret_key);
                if (OQS_SIG_keypair(sig, node.public_key.data(), node.secret_key.data()) == OQS_SUCCESS) node.alive = true;
                OQS_SIG_free(sig);
            }
        }
        return node;
    }
};
