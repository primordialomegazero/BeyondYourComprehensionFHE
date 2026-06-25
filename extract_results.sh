#!/bin/bash
cd BeyondYourComprehensionFHE/build

echo "=========================================="
echo "         B6 HYDRA — RESULTS"
echo "=========================================="
echo ""

echo "=== TEST 1: 6 Engines (36/36) ==="
grep -i "36/36\|6 engines\|encrypt.*decrypt" test_output.log 2>/dev/null || echo "Not found in log"

echo ""
echo "=== TEST 2: Fractal Systems (95/95) ==="
grep -i "95/95\|fractal\|14 party" test_output.log 2>/dev/null || echo "Not found in log"

echo ""
echo "=== TEST 3: TPS Benchmark ==="
grep -i "TPS\|10.2M\|315.9M\|operations" test_output.log benchmark.log 2>/dev/null || echo "Not found in logs"

echo ""
echo "=== API Tests ==="
grep -i "health\|tps\|encrypt" api_test_output.log 2>/dev/null || echo "API log not found"

echo ""
echo "=== Build Status ==="
if [ -f ./b6_hydra ]; then
    echo "✅ b6_hydra executable exists"
    ls -lh ./b6_hydra
else
    echo "❌ b6_hydra not found"
fi

if [ -f ./hydra_gateway ]; then
    echo "✅ hydra_gateway executable exists"
    ls -lh ./hydra_gateway
else
    echo "❌ hydra_gateway not found"
fi

echo ""
echo "=== Test Results Summary ==="
grep -i "passed\|failed\|verified\|healthy" test_output.log 2>/dev/null | tail -20 || echo "No test summary found"

echo ""
echo "=========================================="
echo "         END OF REPORT"
echo "=========================================="
