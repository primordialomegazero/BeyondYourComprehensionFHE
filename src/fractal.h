#pragma once
#include "pozd_fhe.h"
#include <vector>

namespace pozd {

class FractalEngine {
    PozDFHE fhe;
public:
    CT fractal_encrypt(int64_t value, int /*party*/) {
        return fhe.encrypt(value);
    }
    
    int64_t fractal_decrypt(const CT& ct) {
        return fhe.decrypt(ct);
    }
    
    CT chain_add(const std::vector<CT>& cts) {
        if(cts.empty()) return fhe.encrypt(0);
        CT result = cts[0];
        for(size_t i = 1; i < cts.size(); i++)
            result = fhe.add(result, cts[i]);
        return result;
    }
    
    CT chain_multiply(const std::vector<CT>& cts) {
        if(cts.empty()) return fhe.encrypt(1);
        CT result = cts[0];
        for(size_t i = 1; i < cts.size(); i++)
            result = fhe.multiply(result, cts[i]);
        return result;
    }
};

} // namespace pozd
