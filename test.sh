#!/bin/bash
echo "=== B6 HYDRA v7.0 Quick Test ==="
cd build
pkill drogon_gateway 2>/dev/null
./drogon_gateway &
sleep 3
echo "Encrypt test:"
curl -s -X POST http://localhost:8080/manifest -H "Content-Type: application/json" -d '{"action":"encrypt","value":"42"}' | jq .
echo "Health check:"
curl -s http://localhost:8080/health | jq .
pkill drogon_gateway 2>/dev/null
