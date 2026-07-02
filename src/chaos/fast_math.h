#pragma once
#include <cmath>
#include <array>

namespace fast_math {
    constexpr double PHI = 1.6180339887498948482;
    
    // Fast sin via table lookup
    inline double fast_sin(double x) {
        x = std::fmod(x, 2.0 * M_PI);
        double x2 = x * x;
        return x * (1.0 - x2 * (1.0/6.0 - x2 * 1.0/120.0));
    }
    
    // Fast φ^x via approximation
    inline double fast_phi_pow(double exponent) {
        if (exponent < 0) exponent = 0;
        if (exponent > 20) exponent = 20;
        return std::pow(PHI, exponent);  // Fallback to std::pow for accuracy
    }
    
    // Fast tan
    inline double fast_tan(double x) {
        double s = fast_sin(x);
        double c = fast_sin(x + M_PI/2.0);
        if (std::abs(c) < 1e-10) return (s > 0) ? 1e10 : -1e10;
        return s / c;
    }
}
