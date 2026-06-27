#pragma once
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

namespace phi_seal {

constexpr double SCHUMANN = 7.83;

class AutoHarmonizer {
public:
    enum class Mode {
        STREAM,
        BFV_2048,
        BFV_4096,
        CKKS
    };

    struct HarmonyAnalysis {
        double entropy;
        double phi_resonance;
        size_t data_size;
        bool has_decimals;
        bool is_batch;
        Mode recommended_mode;
    };

    static HarmonyAnalysis analyze(const std::string& value) {
        HarmonyAnalysis result;
        result.data_size = value.size();
        result.has_decimals = (value.find('.') != std::string::npos);
        result.is_batch = (value.find(',') != std::string::npos && value.size() > 50);
        
        int unique = 0;
        bool seen[256] = {false};
        for(char c : value) {
            if(!seen[(unsigned char)c]) {
                seen[(unsigned char)c] = true;
                unique++;
            }
        }
        result.entropy = (double)unique / std::max(1.0, (double)value.size());
        
        double sum = 0;
        for(char c : value) sum += (unsigned char)c;
        double avg = sum / std::max(1.0, (double)value.size());
        result.phi_resonance = std::abs(std::sin(avg * 1.6180339887498948482 * SCHUMANN));
        
        // AUTO-DECISION
        if(result.data_size <= 8 && !result.has_decimals && !result.is_batch) {
            result.recommended_mode = Mode::STREAM;
        }
        else if(result.has_decimals) {
            result.recommended_mode = Mode::CKKS;
        }
        else if(result.is_batch) {
            result.recommended_mode = Mode::BFV_4096;
        }
        else {
            result.recommended_mode = Mode::BFV_2048;
        }
        
        return result;
    }
    
    static const char* mode_name(Mode m) {
        switch(m) {
            case Mode::STREAM: return "STREAM (2B, 33M TPS)";
            case Mode::BFV_2048: return "BFV-2048 (31KB, 6ms)";
            case Mode::BFV_4096: return "BFV-4096 (88KB, 78ms)";
            case Mode::CKKS: return "CKKS (2.6MB, 16K slots)";
            default: return "UNKNOWN";
        }
    }
};

} // namespace phi_seal
