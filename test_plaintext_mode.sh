#!/bin/bash
cd ~/build/BeyondYourComprehensionFHE/build

./hydra_gateway &
GATEWAY_PID=$!
sleep 3

echo "=========================================="
echo "   TESTING PLAINTEXT MODE"
echo "=========================================="
echo ""

# Test 1: Plaintext addition
echo "=== 1. Plaintext Addition: 5 + 3 ==="
curl -s -X POST http://localhost:8080/add \
  -H "Content-Type: application/json" \
  -d '{"a": 5, "b": 3}' | jq .

# Test 2: Plaintext multiplication
echo "=== 2. Plaintext Multiplication: 6 * 7 ==="
curl -s -X POST http://localhost:8080/multiply \
  -H "Content-Type: application/json" \
  -d '{"a": 6, "b": 7}' | jq .

# Test 3: Mixed — encrypted + plaintext?
echo "=== 3. Try encrypted + plaintext ==="
ENC=$(curl -s -X POST http://localhost:8080/encrypt -H "Content-Type: application/json" -d '{"value": 5}' | jq -r '.encrypted' 2>/dev/null | sed 's/^φ-//')
echo "Encrypted 5: $ENC"

curl -s -X POST http://localhost:8080/add \
  -H "Content-Type: application/json" \
  -d "{\"a\": \"$ENC\", \"b\": 3}" | jq .

# Test 4: Decrypt the result
echo "=== 4. Check if result is decryptable ==="
RESULT=$(curl -s -X POST http://localhost:8080/add -H "Content-Type: application/json" -d '{"a": 5, "b": 3}' | jq -r '.result')
echo "Result: $RESULT"

curl -s -X POST http://localhost:8080/decrypt \
  -H "Content-Type: application/json" \
  -d "{\"ciphertext\": \"$RESULT\"}" | jq .

kill $GATEWAY_PID 2>/dev/null
