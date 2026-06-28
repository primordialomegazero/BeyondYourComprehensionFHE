#pragma once
#ifdef HAS_OPENFHE
#include <openfhe/pke/openfhe.h>
#endif
#include <cmath>
#include <iostream>

namespace phi_seal {

class PhiOpenFHE {
public:
    bool ready = false;
    
    PhiOpenFHE() {
        // Check if OpenFHE is available via preprocessor
        #ifdef HAS_OPENFHE
        ready = true;
        #endif
    }
};
}
