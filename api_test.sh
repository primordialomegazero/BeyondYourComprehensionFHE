#!/bin/bash
cd BeyondYourComprehensionFHE/build

# Start gateway in background
./hydra_gateway &
GATEWAY_PID=$!
sleep 3

echo "=== API HEALTH ==="
curl -s http://localhost:8080/health | jq . 2>/dev/null || curl -s http://localhost:8080/health

echo ""
echo "=== API TPS ==="
curl -s http://localhost:8080/tps | jq . 2>/dev/null || curl -s http://localhost:8080/tps

echo ""
echo "=== API ENCRYPT ==="
curl -s -X POST http://localhost:8080/encrypt \
  -H "Content-Type: application/json" \
  -d '{"value": 42}' | jq . 2>/dev/null || curl -s -X POST http://localhost:8080/encrypt -H "Content-Type: application/json" -d '{"value": 42}'

kill $GATEWAY_PID 2>/dev/null
