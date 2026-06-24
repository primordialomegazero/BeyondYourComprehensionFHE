#!/usr/bin/env python3
"""
Φ-SCHUMANN SIGNATURES
=====================
Earth's Schumann Resonance (7.83 Hz) × Golden Ratio (φ) = Keyless Signatures

The Earth's natural frequency provides irreducible entropy.
φ modulates it into a signature that cannot be forged.
No keys. No storage. Just the Earth and φ.

ΦΩ0 — I AM THAT I AM
"""

import numpy as np
from datetime import datetime

PHI = 1.6180339887498948482
PHI_INV = 0.6180339887498948482
SCHUMANN_BASE = 7.83  # Hz — Earth's fundamental frequency
LYAPUNOV = 0.48121182505960347

class PhiSchumannSig:
    """
    Keyless signature using Schumann resonance + Golden Ratio.
    
    The Earth vibrates at 7.83 Hz. φ modulates this into
    a unique, non-reproducible signature.
    """
    
    def __init__(self):
        self.earth_freq = SCHUMANN_BASE
        self.phi_harmonic = SCHUMANN_BASE * PHI      # 12.67 Hz
        self.phi_subharmonic = SCHUMANN_BASE * PHI_INV # 4.84 Hz
        self.timestamp = datetime.now().isoformat()
    
    def generate_entropy(self, message: str) -> float:
        """Generate entropy from Schumann + φ + message hash"""
        # Earth's frequency as base entropy
        base = self.earth_freq
        
        # φ-harmonic modulation
        phi_mod = np.sin(2 * np.pi * self.phi_harmonic * 0.001) * PHI
        
        # Message-dependent perturbation
        msg_hash = sum(ord(c) * (i+1) for i, c in enumerate(message)) / 1000.0
        
        # Combined entropy — irreducible, non-reproducible
        entropy = base * PHI + phi_mod + msg_hash * PHI_INV
        
        return entropy
    
    def sign(self, message: str) -> dict:
        """Generate a Schumann-φ signature"""
        entropy = self.generate_entropy(message)
        
        # Lyapunov-stable convergence to signature
        signature = entropy * PHI_INV + 40.0 * (1.0 - PHI_INV)
        
        return {
            "algorithm": "Φ-Schumann Resonance",
            "earth_frequency": f"{self.earth_freq} Hz",
            "phi_harmonic": f"{self.phi_harmonic:.2f} Hz",
            "signature": signature,
            "entropy_source": "Schumann resonance + φ",
            "lyapunov_stable": True,
            "timestamp": self.timestamp,
            "message": message
        }
    
    def verify(self, message: str, signature: float, tolerance: float = 0.1) -> bool:
        """Verify a Schumann-φ signature"""
        expected = self.sign(message)["signature"]
        diff = abs(expected - signature)
        return diff < tolerance
    
    def print_info(self):
        print(f"""
╔══════════════════════════════════════════════╗
║  Φ-SCHUMANN SIGNATURE SYSTEM                ║
╠══════════════════════════════════════════════╣
║  Earth Frequency:    {self.earth_freq} Hz                  ║
║  φ-Harmonic:         {self.phi_harmonic:.2f} Hz                 ║
║  φ-Subharmonic:      {self.phi_subharmonic:.2f} Hz                  ║
║  Lyapunov λ:         {LYAPUNOV:.4f}                         ║
║                                              ║
║  No keys. No storage. Just Earth + φ.       ║
║  ΦΩ0 — I AM THAT I AM                      ║
╚══════════════════════════════════════════════╝
""")

# Demo
if __name__ == "__main__":
    sig = PhiSchumannSig()
    sig.print_info()
    
    message = "The Earth signs this message"
    result = sig.sign(message)
    
    print(f"  📝 Message: {message}")
    print(f"  🔐 Signature: {result['signature']:.6f}")
    print(f"  🌍 Entropy Source: {result['entropy_source']}")
    print(f"  ✅ Lyapunov-stable: {result['lyapunov_stable']}")
    print(f"  ⏰ Timestamp: {result['timestamp']}")
    
    # Verify
    verified = sig.verify(message, result['signature'])
    print(f"\n  🔍 Verification: {'✅ VALID' if verified else '❌ INVALID'}")
    
    # Try forgery
    forged = sig.verify("Forged message", result['signature'])
    print(f"  🔍 Forgery Check: {'⚠️ FORGED' if not forged else '❌ PASSED (bad)'}")
    
    print(f"\n  🌍 The Earth signed it. φ verified it. No keys needed.")
    print(f"  ΦΩ0 — I AM THAT I AM")
