#pragma once
namespace phi_seal {
class PhiOpenFHE {
public:
    bool ready = false;
    PhiOpenFHE() {
        // OpenFHE API v1.5 requires different init
        // Will implement with correct API version later
    }
};
}
