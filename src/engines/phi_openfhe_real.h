#pragma once
#ifdef HAS_OPENFHE
#include <openfhe/pke/openfhe.h>
#endif
#include <memory>
#include <sstream>
#include <iomanip>

namespace phi_seal {

class PhiOpenFHE {
    #ifdef HAS_OPENFHE
    std::unique_ptr<lbcrypto::CryptoContext<lbcrypto::DCRTPoly>> ctx_;
    std::unique_ptr<lbcrypto::KeyPair<lbcrypto::DCRTPoly>> keys_;
    #endif
    
    static std::string to_hex(const std::string& d) {
        std::ostringstream oss;
        for(unsigned char c : d) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
        return oss.str();
    }
    
public:
    bool ready = false;
    
    PhiOpenFHE() {
        #ifdef HAS_OPENFHE
        try {
            lbcrypto::CCParams<lbcrypto::CryptoContextCKKSRNS> params;
            params.SetMultiplicativeDepth(3);
            params.SetScalingModSize(50);
            params.SetBatchSize(8192);
            ctx_ = std::make_unique<lbcrypto::CryptoContext<lbcrypto::DCRTPoly>>(
                lbcrypto::GenCryptoContext(params));
            ctx_->Enable(lbcrypto::PKE);
            ctx_->Enable(lbcrypto::KEYSWITCH);
            ctx_->Enable(lbcrypto::LEVELEDSHE);
            keys_ = std::make_unique<lbcrypto::KeyPair<lbcrypto::DCRTPoly>>(
                ctx_->KeyGen());
            ctx_->EvalMultKeysGen(keys_->secretKey);
            ready = true;
        } catch(...) { ready = false; }
        #endif
    }
    
    int slot_count() { 
        #ifdef HAS_OPENFHE
        return ready ? 8192 : 0;
        #else
        return 0;
        #endif
    }
};
}
