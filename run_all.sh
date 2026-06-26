#!/bin/bash
echo "=========================================="
echo "   B6 HYDRA — FULL TEST SUITE"
echo "   Let the output speak."
echo "=========================================="

./clone.sh
./check.sh
./build.sh
bash test.sh
./api_test.sh > api_test_output.log 2>&1
./benchmark.sh
bash extract_results.sh

echo ""
echo "=========================================="
echo "   ALL TESTS COMPLETE"
echo "   Check these files for raw output:"
echo "   - test_output.log"
echo "   - benchmark.log"
echo "   - api_test_output.log"
echo "=========================================="
