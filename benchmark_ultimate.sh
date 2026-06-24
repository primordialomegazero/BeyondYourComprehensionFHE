#!/bin/bash
echo "🚀 ULTIMATE B6 HYDRA BENCHMARK 🚀"
echo "==================================="
echo "This will run ALL tests sequentially"
echo "Estimated time: 5-10 minutes"
echo ""
read -p "Continue? (y/n): " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]; then
    cd ~/BeyondYourComprehensionFHE/build
    
    echo "[1/5] Smoke Test..."
    ./b6_hydra --test=smoke
    
    echo "[2/5] TPS Benchmark (30s)..."
    ./b6_hydra --test=tps --duration=30
    
    echo "[3/5] Comprehensive (240 ops)..."
    ./b6_hydra --test=comprehensive --ops=240
    
    echo "[4/5] Fractal + Verification..."
    ./b6_hydra --test=fractal --keys=28 --verify
    
    echo "[5/5] Supply Chain Security..."
    ./b6_hydra --test=scs --verify
    
    echo ""
    echo "🎉 BENCHMARK COMPLETE! 🎉"
    echo "Results saved to: benchmark_results.log"
else
    echo "Cancelled."
fi
