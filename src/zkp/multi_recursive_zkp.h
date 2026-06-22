#pragma once

#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/obj_mac.h>
#include <openssl/bn.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

class MultiRecursiveZKP {
public:
    static constexpr double PHI = 1.6180339887498948482;
    static constexpr size_t MAX_DEPTH = 7;
    static constexpr size_t MAX_BRANCHES = 5;  // φ-related branching
    
    struct SchnorrProof {
        std::string commitment_R;
        std::string challenge_c;
        std::string response_s;
        std::string public_key_Y;
        size_t depth;
        size_t branch_id;
        std::vector<SchnorrProof> children;  // RECURSIVE!
    };
    
    struct FractalTree {
        SchnorrProof root;
        size_t total_proofs;
        size_t max_depth;
        bool verified;
    };
    
    // Generate a single Schnorr proof
    static SchnorrProof prove_single(const std::string& data, size_t depth, size_t branch) {
        SchnorrProof proof;
        proof.depth = depth;
        proof.branch_id = branch;
        
        // Standard Schnorr Σ-Protocol on secp256k1
        EC_GROUP* g = EC_GROUP_new_by_curve_name(NID_secp256k1);
        BN_CTX* ctx = BN_CTX_new();
        const BIGNUM* n = EC_GROUP_get0_order(g);
        
        // Private key from data
        std::string hash = sha256(data);
        BIGNUM* x = BN_new();
        BN_hex2bn(&x, hash.c_str());
        BN_mod(x, x, n, ctx);
        
        // Public key
        EC_POINT* Y = EC_POINT_new(g);
        EC_POINT_mul(g, Y, x, nullptr, nullptr, ctx);
        proof.public_key_Y = point_to_hex(g, Y, ctx);
        
        // Random nonce
        BIGNUM* r = BN_new();
        BN_rand_range(r, n);
        EC_POINT* R = EC_POINT_new(g);
        EC_POINT_mul(g, R, r, nullptr, nullptr, ctx);
        proof.commitment_R = point_to_hex(g, R, ctx);
        
        // Fiat-Shamir challenge
        BIGNUM* c = BN_new();
        std::string challenge_input = proof.commitment_R + "||" + proof.public_key_Y;
        BN_hex2bn(&c, sha256(challenge_input).c_str());
        BN_mod(c, c, n, ctx);
        proof.challenge_c = BN_bn2hex(c);
        
        // Response
        BIGNUM* s = BN_new();
        BIGNUM* cx = BN_new();
        BN_mod_mul(cx, c, x, n, ctx);
        BN_mod_add(s, r, cx, n, ctx);
        proof.response_s = BN_bn2hex(s);
        
        // Cleanup
        BN_free(x); BN_free(r); BN_free(c); BN_free(s); BN_free(cx);
        EC_POINT_free(Y); EC_POINT_free(R);
        EC_GROUP_free(g); BN_CTX_free(ctx);
        
        return proof;
    }
    
    // Verify a single proof
    static bool verify_single(const SchnorrProof& proof) {
        EC_GROUP* g = EC_GROUP_new_by_curve_name(NID_secp256k1);
        BN_CTX* ctx = BN_CTX_new();
        const BIGNUM* n = EC_GROUP_get0_order(g);
        
        EC_POINT* R = EC_POINT_new(g);
        EC_POINT* Y = EC_POINT_new(g);
        hex_to_point(g, R, proof.commitment_R);
        hex_to_point(g, Y, proof.public_key_Y);
        
        // Recompute challenge
        std::string challenge_input = proof.commitment_R + "||" + proof.public_key_Y;
        BIGNUM* c = BN_new();
        BN_hex2bn(&c, sha256(challenge_input).c_str());
        BN_mod(c, c, n, ctx);
        
        BIGNUM* stored_c = BN_new();
        BN_hex2bn(&stored_c, proof.challenge_c.c_str());
        
        if (BN_cmp(c, stored_c) != 0) {
            BN_free(c); BN_free(stored_c);
            return false;
        }
        
        // Verify s*G == R + c*Y
        BIGNUM* s = BN_new();
        BN_hex2bn(&s, proof.response_s.c_str());
        
        EC_POINT* sG = EC_POINT_new(g);
        EC_POINT_mul(g, sG, s, nullptr, nullptr, ctx);
        
        EC_POINT* cY = EC_POINT_new(g);
        EC_POINT_mul(g, cY, nullptr, Y, c, ctx);
        
        EC_POINT* RcY = EC_POINT_new(g);
        EC_POINT_add(g, RcY, R, cY, ctx);
        
        bool valid = (EC_POINT_cmp(g, sG, RcY, ctx) == 0);
        
        BN_free(c); BN_free(stored_c); BN_free(s);
        EC_POINT_free(R); EC_POINT_free(Y); EC_POINT_free(sG); EC_POINT_free(cY); EC_POINT_free(RcY);
        EC_GROUP_free(g); BN_CTX_free(ctx);
        
        return valid;
    }
    
    // MULTI-RECURSIVE: Generate fractal tree
    static FractalTree prove_recursive(
        const std::string& data,
        size_t depth = 0,
        size_t max_depth = MAX_DEPTH,
        size_t branch_factor = 3  // φ-related: 3 ≈ φ²
    ) {
        FractalTree tree;
        
        // Root proof
        tree.root = prove_single(data, depth, 0);
        tree.max_depth = depth;
        tree.total_proofs = 1;
        
        // Recursively generate children
        if (depth < max_depth) {
            for (size_t b = 0; b < branch_factor; b++) {
                // Each branch uses a different challenge derived from parent
                std::string branch_data = tree.root.response_s + ":" + 
                                         std::to_string(std::pow(PHI, depth+1)) + ":" +
                                         std::to_string(b) + ":FRACTAL_ZKP";
                
                SchnorrProof child = prove_single(branch_data, depth + 1, b);
                
                // Recursively generate grandchildren
                FractalTree subtree = prove_recursive(branch_data, depth + 1, max_depth, 
                                                      std::max((size_t)2, branch_factor - 1));
                child.children = std::vector<SchnorrProof>{subtree.root};
                
                tree.root.children.push_back(child);
                tree.total_proofs += subtree.total_proofs;
                tree.max_depth = std::max(tree.max_depth, subtree.max_depth);
            }
        }
        
        return tree;
    }
    
    // Verify entire fractal tree
    static bool verify_tree(const SchnorrProof& node) {
        if (!verify_single(node)) return false;
        
        for (const auto& child : node.children) {
            if (!verify_tree(child)) return false;
        }
        
        return true;
    }
    
    static bool verify_tree(const FractalTree& tree) {
        return verify_tree(tree.root);
    }

private:
    static std::string sha256(const std::string& d) {
        unsigned char h[EVP_MAX_MD_SIZE];
        unsigned int l;
        EVP_MD_CTX* c = EVP_MD_CTX_new();
        EVP_DigestInit_ex(c, EVP_sha256(), nullptr);
        EVP_DigestUpdate(c, d.c_str(), d.length());
        EVP_DigestFinal_ex(c, h, &l);
        EVP_MD_CTX_free(c);
        std::stringstream ss;
        for (unsigned int i = 0; i < 32; i++)
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)h[i];
        return ss.str();
    }
    
    static std::string point_to_hex(EC_GROUP* g, EC_POINT* p, BN_CTX* ctx) {
        char* hex = EC_POINT_point2hex(g, p, POINT_CONVERSION_COMPRESSED, ctx);
        std::string r(hex);
        OPENSSL_free(hex);
        return r;
    }
    
    static void hex_to_point(EC_GROUP* g, EC_POINT* p, const std::string& h) {
        EC_POINT_hex2point(g, h.c_str(), p, nullptr);
    }
};
