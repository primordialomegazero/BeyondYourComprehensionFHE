#!/bin/bash
cd BeyondYourComprehensionFHE/build

echo "=== RUNNING B6 HYDRA ==="
echo "Starting at: $(date)"
echo ""

timeout 60 ./b6_hydra 2>&1 | tee test_output.log

echo ""
echo "=== Test Complete ==="
echo "Finished at: $(date)"
