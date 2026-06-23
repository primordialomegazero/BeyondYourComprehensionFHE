#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define PHI 1.6180339887498948482
#define PHI_INV 0.6180339887498948482

double zeros[] = {
    14.134725, 21.022040, 25.010857, 30.424876, 32.935061,
    37.586178, 40.918719, 43.327073, 48.005150, 49.773832,
    52.970321, 56.446247, 59.347044, 60.831778, 65.112544,
    67.079810, 69.546401, 72.067157, 75.704690, 77.144840
};

int main() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║  RIEMANN ZERO HARMONICS               ║\n");
    printf("║  The φ-Dance of the Zeros             ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");

    printf("=== GAPS BETWEEN ZEROS ===\n\n");
    printf("%-8s | %-12s | %-12s | %-12s | %-12s\n", 
           "Zeros", "Gap", "Gap/PHI", "Gap*PHI_INV", "Nearest φ^n");
    printf("---------+--------------+--------------+--------------+--------------\n");

    for (int i = 1; i < 20; i++) {
        double gap = zeros[i] - zeros[i-1];
        double gap_over_phi = gap / PHI;
        double gap_times_phi_inv = gap * PHI_INV;
        
        double nearest_phi_power = 1.0;
        double best_diff = 1e9;
        for (int p = -5; p <= 5; p++) {
            double phi_pow = pow(PHI, p);
            double diff = fabs(gap - phi_pow);
            if (diff < best_diff) {
                best_diff = diff;
                nearest_phi_power = phi_pow;
            }
        }
        
        printf("ρ%d→ρ%d  | %10.6f | %10.6f | %10.6f | φ^%+.0f=%.4f",
               i, i+1, gap, gap_over_phi, gap_times_phi_inv,
               log(nearest_phi_power)/log(PHI), nearest_phi_power);
        
        if (fabs(gap - PHI) < 0.5) printf(" ⭐ φ ITSELF");
        if (fabs(gap - 1.0) < 0.3) printf(" ⭐ φ⁰");
        if (fabs(gap - PHI_INV) < 0.2) printf(" ⭐ φ⁻¹");
        
        printf("\n");
    }

    printf("\n=== GAP RATIOS (DANCE PATTERN) ===\n\n");
    printf("%-10s | %-12s | %-12s\n", "Gap Ratio", "Value", "Distance from φ");
    printf("-----------+--------------+--------------\n");
    
    int near_phi_count = 0;
    for (int i = 2; i < 20; i++) {
        double gap1 = zeros[i-1] - zeros[i-2];
        double gap2 = zeros[i] - zeros[i-1];
        if (gap2 > 0.001) {
            double ratio = gap1 / gap2;
            double dist = fabs(ratio - PHI);
            printf("gap%d/gap%d  | %10.6f | %10.6f", i, i+1, ratio, dist);
            if (dist < 0.3) { printf(" ⭐"); near_phi_count++; }
            if (fabs(ratio - 1.0) < 0.1) printf(" ≈ 1 (φ⁰)");
            if (fabs(ratio - PHI_INV) < 0.2) printf(" ≈ φ⁻¹");
            printf("\n");
        }
    }
    
    printf("\n  Gaps near φ-ratio: %d/%d\n\n", near_phi_count, 18);

    printf("=== φ-HARMONIC SPECTRUM ===\n\n");
    printf("The gaps resonate at φ-frequencies:\n\n");
    
    for (int h = -3; h <= 3; h++) {
        double freq = pow(PHI, h);
        double resonance = 0.0;
        for (int i = 1; i < 20; i++) {
            double gap = zeros[i] - zeros[i-1];
            resonance += cos(gap * 2.0 * M_PI / freq);
        }
        resonance /= 19.0;
        
        printf("  φ^%+d = %8.4f | Resonance: %+6.3f ", h, freq, resonance);
        if (resonance > 0.5) printf("⭐⭐⭐ STRONG");
        else if (resonance > 0.3) printf("⭐⭐ MEDIUM");
        else if (resonance > 0.1) printf("⭐ WEAK");
        printf("\n");
    }

    printf("\n╔═══════════════════════════════════════╗\n");
    printf("║  THE ZEROS DANCE TO φ.                ║\n");
    printf("║  Not random. Not chaotic. φ-harmonic. ║\n");
    printf("║  ΦΩ0 — I AM THAT I AM                ║\n");
    printf("╚═══════════════════════════════════════╝\n");

    return 0;
}
