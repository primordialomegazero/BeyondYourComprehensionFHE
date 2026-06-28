#pragma once
#include "pozd_fhe.h"
#include <string>
#include <vector>

namespace pozd {
namespace scs {

class SupplyChainSecurity {
    PozDFHE fhe;
    
public:
    struct Artifact {
        std::string name;
        CT integrity_hash;
        CT signature;
        int signer_party;
        bool verified;
    };
    
    CT sign_artifact(const std::string& name, int party) {
        int64_t h = 0;
        for(char c : name) h = h * 31 + (int64_t)c;
        int64_t seed = (int64_t)(PHI * (party + 1) * 1000) % 10000;
        return fhe.encrypt(h + seed);
    }
    
    bool verify_artifact(const Artifact& a) {
        int64_t h = 0;
        for(char c : a.name) h = h * 31 + (int64_t)c;
        int64_t seed = (int64_t)(PHI * (a.signer_party + 1) * 1000) % 10000;
        return fhe.decrypt(a.signature) == h + seed;
    }
    
    std::vector<Artifact> build_chain(const std::vector<std::string>& names) {
        std::vector<Artifact> chain;
        for(size_t i = 0; i < names.size(); i++) {
            Artifact a;
            a.name = names[i];
            a.signer_party = i % 14;
            a.signature = sign_artifact(names[i], a.signer_party);
            a.verified = verify_artifact(a);
            chain.push_back(a);
        }
        return chain;
    }
    
    bool verify_chain(const std::vector<Artifact>& chain) {
        for(auto& a : chain)
            if(!verify_artifact(a)) return false;
        return true;
    }
};

} // namespace scs
} // namespace pozd
