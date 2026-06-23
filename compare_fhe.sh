#!/bin/bash
echo "═══════════════════════════════════════════════"
echo "  📊 COMPARISON: BYC vs STANDARD FHE"
echo "═══════════════════════════════════════════════"

# Tingnan kung may actual na SEAL implementation
echo ""
echo "🔍 SEAL implementation:"
find . -name "*.cpp" -o -name "*.h" | xargs grep -E "SEAL|seal" 2>/dev/null | head -20

echo ""
echo "🔍 OpenFHE implementation:"
find . -name "*.cpp" -o -name "*.h" | xargs grep -E "OpenFHE|openfhe" 2>/dev/null | head -20

echo ""
echo "🔍 HElib implementation:"
find . -name "*.cpp" -o -name "*.h" | xargs grep -E "HElib|helib" 2>/dev/null | head -20

echo ""
echo "🔍 Lattigo implementation:"
find . -name "*.go" | xargs grep -E "Lattigo|lattigo" 2>/dev/null | head -20

echo ""
echo "═══════════════════════════════════════════════"
echo "  💡 TANONG: May ACTUAL na FHE math ba?"
echo "═══════════════════════════════════════════════"
