#!/bin/bash
cd ~/build/BeyondYourComprehensionFHE

echo "=== Searching for gateway source files ==="
find . -name "*gateway*" -o -name "*hydra_gateway*" 2>/dev/null

echo ""
echo "=== Checking CMakeLists.txt for gateway ==="
grep -i "gateway" CMakeLists.txt

echo ""
echo "=== Checking src directory ==="
ls -la src/ | grep -i gateway

echo ""
echo "=== Checking byc_server directory ==="
ls -la byc_server/
