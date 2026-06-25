#!/bin/bash
cd ~/build/BeyondYourComprehensionFHE/build

echo "=========================================="
echo "   TESTING HYDRA GATEWAY API"
echo "=========================================="
echo ""

# Start the gateway
echo "=== Starting gateway ==="
./hydra_gateway &
GATEWAY_PID=$!
sleep 3

echo ""
echo "=== Gateway PID: $GATEWAY_PID ==="
echo ""

# Test all endpoints
echo "=== 1. HEALTH CHECK ==="
curl -s http://localhost:8080/health | jq . 2>/dev/null || curl -s http://localhost:8080/health

echo ""
echo "=== 2. TPS METRICS ==="
curl -s http://localhost:8080/tps | jq . 2>/dev/null || curl -s http://localhost:8080/tps

echo ""
echo "=== 3. ENCRYPT ==="
curl -s -X POST http://localhost:8080/encrypt \
  -H "Content-Type: application/json" \
  -d '{"value": 42}' | jq . 2>/dev/null || curl -s -X POST http://localhost:8080/encrypt -H "Content-Type: application/json" -d '{"value": 42}'

echo ""
echo "=== 4. DECRYPT ==="
# Get encrypted value first
ENCRYPTED=$(curl -s -X POST http://localhost:8080/encrypt -H "Content-Type: application/json" -d '{"value": 99}' | grep -o '"[0-9a-f]*"' | head -1 | tr -d '"')
if [ ! -z "$ENCRYPTED" ]; then
    curl -s -X POST http://localhost:8080/decrypt \
      -H "Content-Type: application/json" \
      -d "{\"ciphertext\": \"$ENCRYPTED\"}" | jq . 2>/dev/null || curl -s -X POST http://localhost:8080/decrypt -H "Content-Type: application/json" -d "{\"ciphertext\": \"$ENCRYPTED\"}"
else
    echo "Could not get encrypted value"
fi

echo ""
echo "=== 5. BOOTSTRAP ==="
curl -s -X POST http://localhost:8080/bootstrap \
  -H "Content-Type: application/json" \
  -d '{"ciphertext": "test"}' | jq . 2>/dev/null || curl -s -X POST http://localhost:8080/bootstrap -H "Content-Type: application/json" -d '{"ciphertext": "test"}'

echo ""
echo "=== 6. HOMOMORPHIC ADDITION ==="
curl -s -X POST http://localhost:8080/add \
  -H "Content-Type: application/json" \
  -d '{"a": 10, "b": 20}' | jq . 2>/dev/null || curl -s -X POST http://localhost:8080/add -H "Content-Type: application/json" -d '{"a": 10, "b": 20}'

echo ""
echo "=== 7. HOMOMORPHIC MULTIPLICATION ==="
curl -s -X POST http://localhost:8080/multiply \
  -H "Content-Type: application/json" \
  -d '{"a": 6, "b": 7}' | jq . 2>/dev/null || curl -s -X POST http://localhost:8080/multiply -H "Content-Type: application/json" -d '{"a": 6, "b": 7}'

echo ""
echo "=== Killing gateway ==="
kill $GATEWAY_PID 2>/dev/null
echo "Done!"
