#!/bin/bash
cd ~/build/BeyondYourComprehensionFHE/build

./hydra_gateway &
GATEWAY_PID=$!
sleep 3

echo "=========================================="
echo "   FINAL TEST — WHAT DOES /add ACCEPT?"
echo "=========================================="

echo -e "\n📝 Test A: Plaintext numbers"
curl -s -X POST http://localhost:8080/add -H "Content-Type: application/json" -d '{"a": 5, "b": 3}'

echo -e "\n\n📝 Test B: String numbers (quoted)"
curl -s -X POST http://localhost:8080/add -H "Content-Type: application/json" -d '{"a": "5", "b": "3"}'

echo -e "\n\n📝 Test C: Ciphertext-like string"
curl -s -X POST http://localhost:8080/add -H "Content-Type: application/json" -d '{"a": "abc123", "b": "def456"}'

echo -e "\n\n📝 Test D: Encrypted value (parsed)"
ENC=$(curl -s -X POST http://localhost:8080/encrypt -H "Content-Type: application/json" -d '{"value": 5}' | grep -o '"encrypted":"[^"]*"' | cut -d'"' -f4)
echo "Encrypted 5 = $ENC"
curl -s -X POST http://localhost:8080/add -H "Content-Type: application/json" -d "{\"a\": \"$ENC\", \"b\": 3}"

echo -e "\n\n=========================================="
echo "   INTERPRETATION:"
echo "=========================================="
echo "If Test A returns 8 → accepts plaintext (calculator mode)"
echo "If Test B returns error → expects numbers, not strings"
echo "If Test C returns error → expects valid ciphertext"
echo "If Test D returns 8 → accepts ciphertext (real FHE!)"

kill $GATEWAY_PID 2>/dev/null
