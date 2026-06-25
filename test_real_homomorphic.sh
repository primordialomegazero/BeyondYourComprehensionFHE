#!/bin/bash
cd ~/build/BeyondYourComprehensionFHE/build

./hydra_gateway &
GATEWAY_PID=$!
sleep 3

echo "=========================================="
echo "   REAL HOMOMORPHIC OPERATIONS TEST"
echo "=========================================="
echo ""

echo "=== Step 1: Encrypt 5 ==="
ENCRYPT_A=$(curl -s -X POST http://localhost:8080/encrypt \
  -H "Content-Type: application/json" \
  -d '{"value": 5}')
echo "Ciphertext A: $ENCRYPT_A"

echo ""
echo "=== Step 2: Encrypt 3 ==="
ENCRYPT_B=$(curl -s -X POST http://localhost:8080/encrypt \
  -H "Content-Type: application/json" \
  -d '{"value": 3}')
echo "Ciphertext B: $ENCRYPT_B"

echo ""
echo "=== Step 3: Homomorphic Addition: 5 + 3 ==="
ADD_RESULT=$(curl -s -X POST http://localhost:8080/add \
  -H "Content-Type: application/json" \
  -d "{\"a\": \"$ENCRYPT_A\", \"b\": \"$ENCRYPT_B\"}")
echo "Add result: $ADD_RESULT"

echo ""
echo "=== Step 4: Decrypt Result ==="
DECRYPTED=$(curl -s -X POST http://localhost:8080/decrypt \
  -H "Content-Type: application/json" \
  -d "{\"ciphertext\": \"$ADD_RESULT\"}")
echo "Decrypted: $DECRYPTED"

echo ""
echo "=== Step 5: Homomorphic Multiplication: 5 * 3 ==="
MUL_RESULT=$(curl -s -X POST http://localhost:8080/multiply \
  -H "Content-Type: application/json" \
  -d "{\"a\": \"$ENCRYPT_A\", \"b\": \"$ENCRYPT_B\"}")
echo "Multiply result: $MUL_RESULT"

echo ""
echo "=== Step 6: Decrypt Multiplication Result ==="
DECRYPTED_MUL=$(curl -s -X POST http://localhost:8080/decrypt \
  -H "Content-Type: application/json" \
  -d "{\"ciphertext\": \"$MUL_RESULT\"}")
echo "Decrypted: $DECRYPTED_MUL"

echo ""
echo "=========================================="
echo "   TEST COMPLETE"
echo "=========================================="

kill $GATEWAY_PID 2>/dev/null
