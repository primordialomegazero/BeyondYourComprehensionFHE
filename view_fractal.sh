#!/bin/bash
echo "═══════════════════════════════════════════════"
echo "  🧬 FRACTAL FHE — ACTUAL CODE"
echo "═══════════════════════════════════════════════"
echo ""

# Hanapin ang mga files na may "fractal"
find . -name "*.cpp" -o -name "*.h" -o -name "*.go" | xargs grep -l "fractal\|φ\|phi-harmonic" 2>/dev/null | while read file; do
    echo "📄 FILE: $file"
    echo "───────────────────────────────────────────────"
    cat "$file" | grep -A 15 -B 3 -i "fractal\|φ\|phi-harmonic" 2>/dev/null
    echo ""
    echo "───────────────────────────────────────────────"
done
