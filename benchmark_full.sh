#!/bin/bash
echo "🧬 B6 HYDRA COMPREHENSIVE 🧬"
echo "================================"
cd ~/BeyondYourComprehensionFHE/build

# Test 1: Enc/Dec + Add + Mul
./b6_hydra --test=comprehensive --ops=240

# Test 2: Fractal Systems
./b6_hydra --test=fractal --keys=28 --verify

# Test 3: Cross-Verification
./b6_hydra --test=verify --all-engines

echo "✅ ALL TESTS COMPLETE"
