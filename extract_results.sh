#!/bin/bash
echo "=== Test Results Summary ==="
grep -E "PASS|FAIL|ERROR|TPS|Complete" build/*.log /tmp/hydra_*.txt 2>/dev/null | tail -20
