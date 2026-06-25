#!/bin/bash
cd ~/build/BeyondYourComprehensionFHE/build

echo "=== What's in build directory ==="
ls -la

echo ""
echo "=== Check if gateway was compiled ==="
find . -name "*gateway*" 2>/dev/null

echo ""
echo "=== Check make output for gateway ==="
grep -i "gateway" make_output.log 2>/dev/null || echo "No gateway in make output"

echo ""
echo "=== Check CMake cache for gateway ==="
grep -i "gateway" CMakeCache.txt 2>/dev/null || echo "No gateway in CMake cache"
