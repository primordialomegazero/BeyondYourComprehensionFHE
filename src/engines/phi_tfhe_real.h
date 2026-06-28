#pragma once
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <memory>
#include <sstream>
#include <iomanip>

namespace phi_seal {

class PhiTFHE {
    std::unique_ptr<TFheGateBootstrappingParameterSet> params_;
    std::unique_ptr<TFheGateBootstrappingSecretKeySet> key_;
    const TFheGateBootstrappingCloudKeySet* cloud_key_ = nullptr;
    
    static std::string to_hex(const std::string& d) {
        std::ostringstream oss;
        for(unsigned char c : d) oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
        return oss.str();
    }
    
public:
    bool ready = false;
    
    PhiTFHE() {
        try {
            params_.reset(new_default_gate_bootstrapping_parameters(110));
            key_.reset(new_random_gate_bootstrapping_secret_keyset(params_.get()));
            cloud_key_ = &key_->cloud;
            ready = true;
        } catch(...) { ready = false; }
    }
    
    // Encrypt a single bit (0 or 1)
    std::string encrypt_bit(int bit) {
        if(!ready) return "";
        LweSample* ct = new_gate_bootstrapping_ciphertext(params_.get());
        bootsSymEncrypt(ct, bit ? 1 : 0, key_.get());
        std::string raw((char*)ct, sizeof(LweSample));
        delete_gate_bootstrapping_ciphertext(ct);
        return to_hex(raw);
    }
    
    int slot_count() { return 1; }  // TFHE is single-bit
};
}
