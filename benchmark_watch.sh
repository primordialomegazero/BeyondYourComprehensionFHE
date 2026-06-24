#!/bin/bash
echo "📡 LIVE MONITORING MODE 📡"
echo "================================"
cd ~/BeyondYourComprehensionFHE/build

while true; do
    clear
    echo "🕐 $(date)"
    echo ""
    ./b6_hydra --test=status --engines=all
    echo ""
    echo "Press Ctrl+C to stop"
    sleep 2
done
