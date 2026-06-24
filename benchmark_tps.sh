#!/bin/bash
echo "⚡ B6 HYDRA TPS BENCHMARK ⚡"
echo "================================"
echo "Testing ALL 4 engines for 30 seconds..."
cd ~/BeyondYourComprehensionFHE/build
./b6_hydra --test=tps --duration=30 --engines=all --output=json
echo ""
echo "📊 Results:"
cat tps_results.json | jq '.'
