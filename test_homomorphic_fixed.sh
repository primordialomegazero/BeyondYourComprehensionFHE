#!/bin/bash
cd ~/build/BeyondYourComprehensionFHE/build

./hydra_gateway &
GATEWAY_PID=$!
sleep 3

echo "=========================================="
echo "   HOMOMORPHIC TEST — FIXED"
echo "=========================================="
echo ""

# Function to extract ciphertext
extract_cipher() {
    local response="$1"
    # Try to extract from JSON first
    echo "$response" | jq -r '.encrypted' 2>/dev/null | sed 's/^φ-//' || echo "$response" | grep -o '"encrypted":"[^"]*"' | cut -d'"' -f4 | sed 's/^φ-//'
}

echo "=== Encrypt 5 ==="
RESP_A=$(curl -s -X POST http://localhost:8080/encrypt \
  -H "Content-Type: application/json" \
  -d '{"value": 5}')
echo "Raw: $RESP_A"

CIPHER_A=$(extract_cipher "$RESP_A")
echo "Cipher A: $CIPHER_A"

echo ""
echo "=== Encrypt 3 ==="
RESP_B=$(curl -s -X POST http://localhost:8080/encrypt \
  -H "Content-Type: application/json" \
  -d '{"value": 3}')
echo "Raw: $RESP_B"

CIPHER_B=$(extract_cipher "$RESP_B")
echo "Cipher B: $CIPHER_B"

echo ""
echo "=== Addition: 5 + 3 ==="
ADD_RESULT=$(curl -s -X POST http://localhost:8080/add \
  -H "Content-Type: application/json" \
  -d "{\"a\": \"$CIPHER_A\", \"b\": \"$CIPHER_B\"}")
echo "Add Result: $ADD_RESULT"

echo ""
echo "=== Multiplication: 5 * 3 ==="
MUL_RESULT=$(curl -s -X POST http://localhost:8080/multiply \
  -H "Content-Type: application/json" \
  -d "{\"a\": \"$CIPHER_A\", \"b\": \"$CIPHER_B\"}")
echo "Multiply Result: $MUL_RESULT"

echo ""
echo "=== Decrypt Addition Result ==="
DECRYPT_ADD=$(curl -s -X POST http://localhost:8080/decrypt \
  -H "Content-Type: application/json" \
  -d "{\"ciphertext\": \"$ADD_RESULT\"}")
echo "Decrypted Add: $DECRYPT_ADD"

echo ""
echo "=== Decrypt Multiplication Result ==="
DECRYPT_MUL=$(curl -s -X POST http://localhost:8080/decrypt \
  -H "Content-Type: application/json" \
  -d "{\"ciphertext\": \"$MUL_RESULT\"}")
echo "Decrypted Mul: $DECRYPT_MUL"

kill $GATEWAY_PID 2>/dev/null
