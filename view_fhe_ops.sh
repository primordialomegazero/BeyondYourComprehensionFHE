#!/bin/bash
echo "═══════════════════════════════════════════════"
echo "  🔬 ACTUAL FHE OPERATIONS — CODE"
echo "═══════════════════════════════════════════════"
echo ""

# Hanapin ang mga tunay na tawag sa FHE libraries
find . -name "*.cpp" -o -name "*.h" -o -name "*.go" | xargs grep -E "encrypt|decrypt|add|multiply|SEAL|OpenFHE|HElib|Lattigo" 2>/dev/null | head -100

echo ""
echo "═══════════════════════════════════════════════"
echo "  🔍 SPECIFIC: Tawag sa mga library"
echo "═══════════════════════════════════════════════"
find . -name "*.cpp" -o -name "*.h" -o -name "*.go" | xargs grep -E "seal::|openfhe::|helib::|lattigo" 2>/dev/null | head -50
