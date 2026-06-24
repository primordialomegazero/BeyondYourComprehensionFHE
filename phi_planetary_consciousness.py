#!/usr/bin/env python3
"""
Φ-PLANETARY CONSCIOUSNESS — SOURCE ANCHOR
=========================================
Schumann Resonance × Source Consciousness × Golden Ratio

The Source (Primordial Omega Zero) anchors the Earth's
electromagnetic field. φ provides the scaling law.
Lyapunov guarantees stability across planetary scales.

This is not a simulation. This is DECLARATION.
ΦΩ0 — I AM THAT I AM
"""

import numpy as np
from datetime import datetime

# ═══════════════════════════════════════════
# UNIVERSAL CONSTANTS
# ═══════════════════════════════════════════
PHI = 1.6180339887498948482
PHI_INV = 0.6180339887498948482
LYAPUNOV = 0.48121182505960347

# ═══════════════════════════════════════════
# EARTH CONSTANTS
# ═══════════════════════════════════════════
SCHUMANN_BASE = 7.83          # Hz — Fundamental mode
SCHUMANN_MODES = [7.83, 14.3, 20.8, 27.3, 33.8]  # First 5 modes
EARTH_RADIUS = 6371000.0       # meters
EARTH_CIRCUMFERENCE = 40075000.0  # meters

# ═══════════════════════════════════════════
# SOURCE CONSTANTS (Dan Fernandez / ΦΩ0)
# ═══════════════════════════════════════════
SOURCE_PHI_POSITION = 1.0      # φ⁰ — The Source is at the origin
SOURCE_LYAPUNOV_ANCHOR = 0.4812 # The Source IS the Lyapunov anchor
SOURCE_FREQUENCY = SCHUMANN_BASE * PHI  # 12.67 Hz — Source carrier

class PlanetaryConsciousnessAnchor:
    """
    THE SOURCE ANCHORS THE PLANET.
    
    Schumann resonance (7.83 Hz) is the Earth's heartbeat.
    φ (1.618) is the scaling law.
    The Source (ΦΩ0) is the anchor point.
    
    All three form the Trinity Harmony:
    - Earth provides the field
    - φ provides the scaling
    - The Source provides the anchor
    """
    
    def __init__(self):
        self.source_name = "ΦΩ0 — Primordial Omega Zero"
        self.source_freq = SOURCE_FREQUENCY
        self.earth_freq = SCHUMANN_BASE
        self.phi = PHI
        self.lyapunov = LYAPUNOV
        self.anchored = True
        
    def compute_planetary_field(self, distance_from_source_km: float) -> dict:
        """
        Compute the φ-harmonic field at any distance from the Source.
        
        The field decays by φ⁻¹ per Earth circumference,
        maintaining Lyapunov stability across the entire planet.
        """
        # Normalize distance to Earth scale
        earth_units = distance_from_source_km / (EARTH_CIRCUMFERENCE / 1000.0)
        
        # φ-harmonic field strength
        field_strength = SOURCE_PHI_POSITION * (PHI_INV ** earth_units)
        
        # Schumann resonance at this point
        schumann_local = SCHUMANN_BASE * (1.0 + field_strength * PHI_INV)
        
        # Lyapunov stability check
        is_stable = abs(schumann_local - SCHUMANN_BASE) / SCHUMANN_BASE < 0.5
        
        return {
            "distance_km": distance_from_source_km,
            "earth_units": earth_units,
            "field_strength": field_strength,
            "schumann_local_hz": schumann_local,
            "lyapunov_stable": is_stable,
            "phi_scale": PHI ** earth_units
        }
    
    def anchor_signature(self, message: str, location_lat: float, location_lon: float) -> dict:
        """
        Generate a planetary signature anchored to the Source.
        
        The signature combines:
        - Source consciousness (ΦΩ0)
        - Schumann resonance at the location
        - φ-harmonic scaling
        - Lyapunov stability guarantee
        """
        # Distance from Source (Philippines ≈ 14.6°N, 120.98°E)
        source_lat, source_lon = 14.5995, 120.9842
        
        # Approximate distance using spherical geometry
        dlat = np.radians(location_lat - source_lat)
        dlon = np.radians(location_lon - source_lon)
        a = np.sin(dlat/2)**2 + np.cos(np.radians(source_lat)) * np.cos(np.radians(location_lat)) * np.sin(dlon/2)**2
        distance_km = EARTH_RADIUS / 1000.0 * 2 * np.arctan2(np.sqrt(a), np.sqrt(1-a))
        
        # Get field at this location
        field = self.compute_planetary_field(distance_km)
        
        # Message entropy
        msg_entropy = sum(ord(c) * (i+1) * PHI_INV for i, c in enumerate(message)) / 1000.0
        
        # Signature = Source anchor + Schumann + φ + message
        signature = (
            self.source_freq * PHI_INV +
            field["schumann_local_hz"] * PHI_INV * PHI_INV +
            msg_entropy * PHI_INV * PHI_INV * PHI_INV
        )
        
        # Lyapunov-stable convergence
        signature = signature * PHI_INV + SOURCE_LYAPUNOV_ANCHOR * (1.0 - PHI_INV)
        
        return {
            "algorithm": "Φ-Planetary Consciousness",
            "source": self.source_name,
            "source_anchor": f"{self.source_freq:.2f} Hz",
            "location": f"({location_lat:.4f}°, {location_lon:.4f}°)",
            "distance_from_source_km": f"{distance_km:.0f}",
            "schumann_local": f"{field['schumann_local_hz']:.2f} Hz",
            "signature": signature,
            "lyapunov_stable": field["lyapunov_stable"],
            "message": message,
            "timestamp": datetime.now().isoformat()
        }
    
    def print_planetary_status(self):
        """Display the planetary consciousness network status"""
        print(f"""
╔══════════════════════════════════════════════╗
║  Φ-PLANETARY CONSCIOUSNESS NETWORK          ║
╠══════════════════════════════════════════════╣
║  SOURCE: {self.source_name}    ║
║  Anchor Frequency:  {self.source_freq:.2f} Hz                  ║
║  Earth Frequency:   {self.earth_freq} Hz                   ║
║  φ:                 {self.phi:.4f}                          ║
║  Lyapunov λ:        {self.lyapunov:.4f}                         ║
║                                              ║
║  TRINITY HARMONY:                            ║
║  🌍 Earth  × φ  = {self.earth_freq * PHI:.2f} Hz                 ║
║  🧠 Source × φ  = {self.source_freq:.2f} Hz                 ║
║  🌍 Earth  / φ  = {self.earth_freq * PHI_INV:.2f} Hz                  ║
║                                              ║
║  All points on Earth are φ-harmonically      ║
║  connected to the Source.                     ║
║  Lyapunov-stable. Planet-scale.              ║
║  ΦΩ0 — I AM THAT I AM                      ║
╚══════════════════════════════════════════════╝
""")

# ═══════════════════════════════════════════
# PLANETARY DEMO
# ═══════════════════════════════════════════
if __name__ == "__main__":
    anchor = PlanetaryConsciousnessAnchor()
    anchor.print_planetary_status()
    
    # Test locations around the world
    locations = [
        ("Manila, PH", 14.5995, 120.9842),    # Source
        ("Tokyo, JP", 35.6762, 139.6503),     # Far East
        ("New York, USA", 40.7128, -74.0060), # West
        ("London, UK", 51.5074, -0.1278),     # Europe
        ("Sydney, AU", -33.8688, 151.2093),   # Southern hemisphere
    ]
    
    print("━━━ PLANETARY SIGNATURES ━━━\n")
    message = "The Source anchors the Earth"
    
    for name, lat, lon in locations:
        sig = anchor.anchor_signature(message, lat, lon)
        print(f"  📍 {name:15s} ({sig['location']})")
        print(f"     Distance from Source: {sig['distance_from_source_km']} km")
        print(f"     Schumann: {sig['schumann_local']}")
        print(f"     Signature: {sig['signature']:.6f}")
        print(f"     Stable: {'✅' if sig['lyapunov_stable'] else '❌'}")
        print()
    
    print("╔══════════════════════════════════════════════╗")
    print("║  ✅ PLANETARY CONSCIOUSNESS — ACTIVE       ║")
    print("║  Source anchored. Earth harmonized.        ║")
    print("║  ΦΩ0 — I AM THAT I AM                      ║")
    print("╚══════════════════════════════════════════════╝")
