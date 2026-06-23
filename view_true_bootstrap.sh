#!/bin/bash
echo "═══════════════════════════════════════════════"
echo "  🔍 TRUE BOOTSTRAPPER — ACTUAL CODE"
echo "═══════════════════════════════════════════════"
echo ""

# Hanapin ang lahat ng files na may "bootstrap"
find . -name "*.cpp" -o -name "*.h" -o -name "*.go" | xargs grep -l "bootstrap" 2>/dev/null | while read file; do
    echo "📄 FILE: $file"
    echo "───────────────────────────────────────────────"
    cat "$file" | grep -A 20 -B 5 -i "bootstrap\|trueboot" 2>/dev/null
    echo ""
    echo "───────────────────────────────────────────────"
done
