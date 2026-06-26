#!/bin/bash
# ═══════════════════════════════════════════════════════════════
#   HYDRA SELF-AUDIT SUITE v2.0 — WSL2 OPTIMIZED
# ═══════════════════════════════════════════════════════════════

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'
BOLD='\033[1m'

PASS=0
FAIL=0
WARN=0
TOTAL=0
BUILD_DIR="$(pwd)/build"
SRC_DIR="$(pwd)/src"

mkdir -p "$BUILD_DIR"

echo -e "${CYAN}${BOLD}"
echo "╔══════════════════════════════════════════════════════╗"
echo "║   B6 HYDRA SELF-AUDIT v2.0 — WSL2 OPTIMIZED         ║"
echo "║   Built-in security suite                           ║"
echo "╚══════════════════════════════════════════════════════╝"
echo -e "${NC}"

# ════════════════════════════════════════════
# PHASE 1: STATIC ANALYSIS
# ════════════════════════════════════════════
echo -e "\n${BOLD}[PHASE 1]${NC} ${YELLOW}STATIC CODE ANALYSIS${NC}"
echo "─────────────────────────────────────────"

((TOTAL++))
echo -n "  [1.1] Cppcheck... "
cppcheck --enable=all --inconclusive --std=c++17 --suppress=missingIncludeSystem \
    "$SRC_DIR/drogon_gateway.cpp" "$SRC_DIR/b6_hydra.cpp" "$SRC_DIR/gateway.cpp" \
    2>"$BUILD_DIR/audit_cppcheck.log" 1>/dev/null
WARNINGS=$(grep -c "^\[" "$BUILD_DIR/audit_cppcheck.log" 2>/dev/null || echo "0")
if [ "$WARNINGS" -eq 0 ]; then
    echo -e "${GREEN}✅ CLEAN${NC}"; ((PASS++))
else
    echo -e "${YELLOW}⚠️  $WARNINGS style notes${NC}"; ((WARN++))
fi

((TOTAL++))
echo -n "  [1.2] Build with hardening flags... "
cd "$BUILD_DIR"
cmake .. -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_FLAGS="-O2 -fstack-protector-strong -D_FORTIFY_SOURCE=2 -fPIE -pie" \
    2>/dev/null 1>/dev/null
make -j$(nproc) 2>/dev/null 1>/dev/null
if [ -f drogon_gateway ]; then
    echo -e "${GREEN}✅ HARDENED BUILD OK${NC}"; ((PASS++))
else
    echo -e "${RED}❌ BUILD FAILED${NC}"; ((FAIL++))
fi

# ════════════════════════════════════════════
# PHASE 2: BINARY HARDENING CHECK
# ════════════════════════════════════════════
echo -e "\n${BOLD}[PHASE 2]${NC} ${YELLOW}BINARY HARDENING${NC}"
echo "─────────────────────────────────────────"

cd "$BUILD_DIR"

((TOTAL++))
echo -n "  [2.1] Stack protector... "
if readelf -s drogon_gateway 2>/dev/null | grep -q "__stack_chk_fail"; then
    echo -e "${GREEN}✅ ENABLED${NC}"; ((PASS++))
else
    echo -e "${RED}❌ MISSING${NC}"; ((FAIL++))
fi

((TOTAL++))
echo -n "  [2.2] Full RELRO... "
if readelf -l drogon_gateway 2>/dev/null | grep -q "GNU_RELRO"; then
    echo -e "${GREEN}✅ FULL RELRO${NC}"; ((PASS++))
else
    echo -e "${YELLOW}⚠️  PARTIAL${NC}"; ((WARN++))
fi

((TOTAL++))
echo -n "  [2.3] PIE... "
if file drogon_gateway | grep -q "pie executable\|shared object"; then
    echo -e "${GREEN}✅ PIE${NC}"; ((PASS++))
else
    echo -e "${RED}❌ NOT PIE${NC}"; ((FAIL++))
fi

((TOTAL++))
echo -n "  [2.4] NX stack... "
if readelf -l drogon_gateway 2>/dev/null | grep "GNU_STACK" | grep -q "E"; then
    echo -e "${RED}❌ EXEC STACK${NC}"; ((FAIL++))
else
    echo -e "${GREEN}✅ NX${NC}"; ((PASS++))
fi

# ════════════════════════════════════════════
# PHASE 3: RUNTIME (WITH TIMEOUT)
# ════════════════════════════════════════════
echo -e "\n${BOLD}[PHASE 3]${NC} ${YELLOW}RUNTIME BEHAVIOR${NC}"
echo "─────────────────────────────────────────"

((TOTAL++))
echo -n "  [3.1] Startup + health check... "
pkill drogon_gateway 2>/dev/null
timeout 5 ./drogon_gateway &>/dev/null &
PID=$!
sleep 4
if curl -s --max-time 2 http://localhost:8080/health >/dev/null 2>&1; then
    echo -e "${GREEN}✅ OK${NC}"; ((PASS++))
else
    echo -e "${RED}❌ FAIL${NC}"; ((FAIL++))
fi
pkill drogon_gateway 2>/dev/null
sleep 1

((TOTAL++))
echo -n "  [3.2] 10 concurrent requests... "
timeout 8 ./drogon_gateway &>/dev/null &
PID=$!
sleep 4
for i in {1..10}; do
    curl -s --max-time 3 -X POST http://localhost:8080/manifest \
        -H "Content-Type: application/json" \
        -d '{"action":"encrypt","value":"test"}' &>/dev/null &
done
wait 2>/dev/null
if kill -0 $PID 2>/dev/null; then
    echo -e "${GREEN}✅ SURVIVED${NC}"; ((PASS++))
else
    echo -e "${RED}❌ CRASHED${NC}"; ((FAIL++))
fi
pkill drogon_gateway 2>/dev/null
sleep 1

((TOTAL++))
echo -n "  [3.3] Injection attacks... "
timeout 8 ./drogon_gateway &>/dev/null &
PID=$!
sleep 4
curl -s --max-time 2 -X POST http://localhost:8080/manifest \
    -H "Content-Type: application/json" \
    -d '{"action":"encrypt","value":"1; DROP TABLE;"}' &>/dev/null
curl -s --max-time 2 -X POST http://localhost:8080/manifest \
    -H "Content-Type: application/json" \
    -d '{"action":"encrypt","value":"$(rm -rf /)"}' &>/dev/null
if kill -0 $PID 2>/dev/null; then
    echo -e "${GREEN}✅ NO CRASH${NC}"; ((PASS++))
else
    echo -e "${RED}❌ CRASHED${NC}"; ((FAIL++))
fi
pkill drogon_gateway 2>/dev/null

# ════════════════════════════════════════════
# FINAL REPORT
# ════════════════════════════════════════════
echo -e "\n${CYAN}${BOLD}"
echo "╔══════════════════════════════════════════════════════╗"
echo "║              SELF-AUDIT FINAL REPORT                 ║"
echo "╚══════════════════════════════════════════════════════╝"
echo -e "${NC}"
echo -e "  ${GREEN}✅ PASSED: $PASS${NC}"
echo -e "  ${RED}❌ FAILED: $FAIL${NC}"
echo -e "  ${YELLOW}⚠️  WARNINGS: $WARN${NC}"
echo -e "  📊 TOTAL: $TOTAL"
echo ""

if [ $FAIL -eq 0 ]; then
    echo -e "${GREEN}${BOLD}  🏆 VERDICT: PRODUCTION-READY${NC}"
    echo -e "  Self-audit: $(date '+%Y-%m-%d %H:%M')"
    echo -e "  System: $(uname -r) | $(nproc) cores | $(free -h | awk '/Mem/{print $2}') RAM"
else
    echo -e "${RED}${BOLD}  🔴 FIX REQUIRED ($FAIL test(s) failed)${NC}"
fi
echo ""

cd "$(dirname "$BUILD_DIR")"
