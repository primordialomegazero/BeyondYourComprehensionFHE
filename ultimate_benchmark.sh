#!/bin/bash
cd ~/BeyondYourComprehensionFHE/build && \
echo "🔥 STARTING BENCHMARK $(date)" && \
./b6_hydra --test=all --benchmark=full --output=json --duration=60 && \
echo "✅ DONE $(date)" && \
cat benchmark_results.json | jq '.summary'
