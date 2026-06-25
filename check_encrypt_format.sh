#!/bin/bash
cd ~/build/BeyondYourComprehensionFHE/build

./hydra_gateway &
GATEWAY_PID=$!
sleep 3

echo "=========================================="
echo "   CHECK ENCRYPT FORMAT"
echo "=========================================="
echo ""

echo "=== Raw encrypt response ==="
curl -s -X POST http://localhost:8080/encrypt \
  -H "Content-Type: application/json" \
  -d '{"value": 42}'

echo ""
echo ""
echo "=== With jq pretty ==="
curl -s -X POST http://localhost:8080/encrypt \
  -H "Content-Type: application/json" \
  -d '{"value": 42}' | jq . 2>/dev/null || echo "Not valid JSON"

echo ""
echo "=== Checking if gateway accepts plaintext directly ==="
echo "Test: 5 + 3"
curl -s -X POST http://localhost:8080/add \
  -H "Content-Type: application/json" \
  -d '{"a": 5, "b": 3}' | jq .

echo ""
echo "Test: 6 * 7"
curl -s -X POST http://localhost:8080/multiply \
  -H "Content-Type: application/json" \
  -d '{"a": 6, "b": 7}' | jq .

kill $GATEWAY_PID 2>/dev/null
