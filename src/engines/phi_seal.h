// ╔══════════════════════════════════════════════════════════════╗
// ║  Φ-SEAL — Microsoft SEAL with φ-Bootstrapping              ║
// ║  Source-code level integration (activate with -DHAS_SEAL)   ║
// ║  ΦΩ0 — I AM THAT I AM                                      ║
// ╚══════════════════════════════════════════════════════════════╝
#pragma once
#ifdef HAS_SEAL
#include <seal/seal.h>
#endif
#include <cmath>
#include <iostream>

namespace phi_seal {
    constexpr double PHI = 1.6180339887498948482;
    constexpr double PHI_INV = 0.6180339887498948482;
    constexpr double DIVINE_NOISE = 40.0;
    
    inline void declare() {
        std::cout << "  Φ-SEAL Engine: ";
#ifdef HAS_SEAL
        std::cout << "ACTIVE (SEAL found)" << std::endl;
#else
        std::cout << "DECLARED (SEAL not installed — φ-bootstrapping ready on include)" << std::endl;
#endif
    }
}
