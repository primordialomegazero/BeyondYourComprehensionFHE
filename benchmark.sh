#!/bin/bash
cd BeyondYourComprehensionFHE/build

echo "=== 30-SECOND TPS BENCHMARK ==="
echo "Starting: $(date)"
echo ""

# If there's a benchmark executable
if [ -f ./benchmark ]; then
    timeout 35 ./benchmark 2>&1 | tee benchmark.log
else
    # Try the main binary with benchmark flag
    timeout 35 ./b6_hydra --benchmark 2>&1 | tee benchmark.log
fi

echo ""
echo "=== Benchmark Complete ==="
echo "Finished: $(date)"
