#pragma once
#include <tfhe/tfhe.h>
#include <tfhe/tfhe_io.h>
#include <cmath>
#include <iostream>
#include <memory>

namespace phi_seal {

class PhiTFHE {
    std::unique_ptr<TFheGateBootstrappingParameterSet> params_;
    std::unique_ptr<TFheGateBootstrappingSecretKeySet> key_;
    
public:
    bool ready = false;
    constexpr static double PHI = 1.6180339887498948482;
    
    PhiTFHE() {
        try {
            params_.reset(new_default_gate_bootstrapping_parameters(110));
            key_.reset(new_random_gate_bootstrapping_secret_keyset(params_.get()));
            ready = true;
        } catch(...) { ready = false; }
    }
    
    ~PhiTFHE() = default;
};
}
