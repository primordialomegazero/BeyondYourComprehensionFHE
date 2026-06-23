#!/bin/bash
echo "═══════════════════════════════════════════════"
echo "  ⚡ TPS BENCHMARK VERIFICATION"
echo "═══════════════════════════════════════════════"
echo ""

# Tingnan kung ano ang ACTUALLY binibilang ng benchmark
find . -name "test3*" -o -name "*tps*" | xargs cat 2>/dev/null | grep -E "count|ops|TPS|for|while|loop" -A 5 -B 5

echo ""
echo "═══════════════════════════════════════════════"
echo "  🔍 ACTUAL COUNTING MECHANISM"
echo "═══════════════════════════════════════════════"
find . -name "*.cpp" | xargs grep -E "atomic|counter|\+\+|increment" -A 3 -B 3 2>/dev/null | head -50
