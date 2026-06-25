#!/bin/bash
cd BeyondYourComprehensionFHE
echo "=== Directory Structure ==="
ls -la
echo ""
echo "=== Source Files ==="
find src -name "*.cpp" | wc -l
find include -name "*.h" | wc -l
echo ""
echo "=== Dependencies ==="
grep -r "seal\|openfhe\|tfhe\|zama" CMakeLists.txt 2>/dev/null || echo "No deps found in CMakeLists"
echo ""
echo "=== Main Files ==="
find . -name "main.cpp" -o -name "b6_hydra.cpp" 2>/dev/null
