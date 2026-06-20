#pragma once
#ifdef HAS_OPENFHE
#include <openfhe/pke/openfhe.h>
#endif
#include <cmath>
#include <iostream>

namespace phi_openfhe {
    constexpr double PHI = 1.6180339887498948482;
    constexpr double PHI_INV = 0.6180339887498948482;
    
    inline void declare() {
        std::cout << "  Φ-OpenFHE Engine: ";
#ifdef HAS_OPENFHE
        std::cout << "ACTIVE (CKKS with φ-mirror healing)" << std::endl;
#else
        std::cout << "DECLARED (install OpenFHE to activate)" << std::endl;
#endif
    }
}
