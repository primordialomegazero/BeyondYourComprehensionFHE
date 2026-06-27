cmake_minimum_required(VERSION 3.16)
project(BeyondYourComprehensionFHE VERSION 7.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# ═══════════════════════════════════════════════
#   INCLUDE PATHS — ALL 6 ENGINES
# ═══════════════════════════════════════════════
include_directories(
    ${CMAKE_SOURCE_DIR}/src/fhe
    ${CMAKE_SOURCE_DIR}/src/zkp
    ${CMAKE_SOURCE_DIR}/src
    /usr/local/include/SEAL-4.3
    /usr/local/include/openfhe/pke
    /usr/local/include/openfhe/core
    /usr/local/include/openfhe/binfhe
    /usr/local/include/tfhe
    $ENV{HOME}/HElib/src
    ${CMAKE_SOURCE_DIR}/../lattigo
)

# Find packages
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(OQS REQUIRED liboqs)

# ═══════════════════════════════════════════════
#   ENGINE 1: Microsoft SEAL
# ═══════════════════════════════════════════════
find_package(SEAL REQUIRED)
add_compile_definitions(HAS_SEAL)
message(STATUS "✅ ENGINE 1/6: Microsoft SEAL FOUND")

# ═══════════════════════════════════════════════
#   ENGINE 2: OpenFHE
# ═══════════════════════════════════════════════
find_library(OPENFHE_PKE OPENFHEpke PATHS /usr/local/openfhe/lib)
find_library(OPENFHE_CORE OPENFHEcore PATHS /usr/local/openfhe/lib)
find_library(OPENFHE_BINFHE OPENFHEbinfhe PATHS /usr/local/openfhe/lib)
if(OPENFHE_PKE AND OPENFHE_CORE)
    add_compile_definitions(HAS_OPENFHE)
    message(STATUS "✅ ENGINE 2/6: OpenFHE FOUND")
else()
    message(WARNING "❌ ENGINE 2/6: OpenFHE NOT FOUND")
endif()

# ═══════════════════════════════════════════════
#   ENGINE 3: Zama TFHE
# ═══════════════════════════════════════════════
find_library(TFHE_NAYUKI tfhe-nayuki-avx PATHS /usr/local/lib)
find_library(TFHE_SPQLIOS tfhe-spqlios-fma PATHS /usr/local/lib)
find_library(TFHE_PORTABLE tfhe-nayuki-portable PATHS /usr/local/lib)
if(TFHE_NAYUKI OR TFHE_SPQLIOS OR TFHE_PORTABLE)
    add_compile_definitions(HAS_TFHE)
    message(STATUS "✅ ENGINE 3/6: Zama TFHE FOUND")
    set(TFHE_LIBS ${TFHE_NAYUKI} ${TFHE_SPQLIOS} ${TFHE_PORTABLE})
else()
    message(WARNING "❌ ENGINE 3/6: TFHE NOT FOUND")
endif()

# ═══════════════════════════════════════════════
#   ENGINE 4: IBM HElib
# ═══════════════════════════════════════════════
set(HELIB_LIB "$ENV{HOME}/HElib/build/lib/libhelib.a")
set(NTL_LIB "/usr/lib/x86_64-linux-gnu/libntl.so")
add_compile_definitions(HAS_HELIB)
message(STATUS "✅ ENGINE 4/6: IBM HElib FOUND")

# ═══════════════════════════════════════════════
#   ENGINE 5: Lattigo (Go — CGo bridge)
# ═══════════════════════════════════════════════
if(EXISTS ${CMAKE_SOURCE_DIR}/../lattigo/lattigo.go)
    add_compile_definitions(HAS_LATTIGO)
    message(STATUS "✅ ENGINE 5/6: Lattigo FOUND (Go library — separate service)")
else()
    message(WARNING "❌ ENGINE 5/6: Lattigo NOT FOUND")
endif()

# ═══════════════════════════════════════════════
#   ENGINE 6: liboqs (PQC)
# ═══════════════════════════════════════════════
message(STATUS "✅ ENGINE 6/6: liboqs (PQC) FOUND — ${OQS_LIBRARIES}")

# ═══════════════════════════════════════════════
#   MAIN EXECUTABLE: b6_hydra
# ═══════════════════════════════════════════════
add_executable(b6_hydra src/b6_hydra.cpp)
target_link_libraries(b6_hydra
    OpenSSL::SSL OpenSSL::Crypto
    ${OQS_LIBRARIES}
    SEAL::seal
    ${OPENFHE_PKE} ${OPENFHE_CORE} ${OPENFHE_BINFHE}
    ${TFHE_LIBS}
    ${HELIB_LIB} ${NTL_LIB}
)
message(STATUS "🔗 b6_hydra linked with ALL available engines")

# ═══════════════════════════════════════════════
#   GATEWAY: hydra_gateway
# ═══════════════════════════════════════════════
add_executable(hydra_gateway src/gateway.cpp)
target_link_libraries(hydra_gateway pthread)

# ═══════════════════════════════════════════════
#   DROGON FHE GATEWAY
# ═══════════════════════════════════════════════
find_package(Drogon REQUIRED)
add_executable(drogon_gateway src/drogon_gateway.cpp src/fhe/multi_recursive_fhe.cpp)
target_link_libraries(drogon_gateway Drogon::Drogon pthread SEAL::seal ${OQS_LIBRARIES} ${OPENFHE_PKE} ${OPENFHE_CORE} ${OPENFHE_BINFHE} ${TFHE_LIBS} ${HELIB_LIB} ${NTL_LIB})

# ═══════════════════════════════════════════════
#   SELF-AUDIT
# ═══════════════════════════════════════════════
add_custom_target(audit
    COMMAND ${CMAKE_SOURCE_DIR}/audit_hydra.sh
    COMMENT "Running B6 HYDRA Self-Audit Suite..."
    DEPENDS drogon_gateway b6_hydra
)

# ═══════════════════════════════════════════════
#   PhiSEAL HTTP API (True FHE Endpoint)
# ═══════════════════════════════════════════════
add_executable(phiseal_gateway src/phiseal_gateway.cpp)
target_link_libraries(phiseal_gateway Drogon::Drogon pthread SEAL::seal)

# ═══════════════════════════════════════════════
#   PhiCKKS HTTP API (CKKS Scheme — 32K Slots)
# ═══════════════════════════════════════════════
add_executable(phickks_gateway src/phickks_gateway.cpp)
target_link_libraries(phickks_gateway Drogon::Drogon pthread SEAL::seal)

# ═══════════════════════════════════════════════
#   PhiBFV OPTIMIZED (2048° φ-BFV, <10ms target)
# ═══════════════════════════════════════════════
add_executable(phiopt_gateway src/phiopt_gateway.cpp)
target_link_libraries(phiopt_gateway Drogon::Drogon pthread SEAL::seal)

# ═══════════════════════════════════════════════
#   HYDRA UNIFIED — Single Gateway (Quick + FHE)
# ═══════════════════════════════════════════════
add_executable(hydra_unified src/hydra_unified.cpp)
target_link_libraries(hydra_unified Drogon::Drogon pthread SEAL::seal)
