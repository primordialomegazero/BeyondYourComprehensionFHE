cmake_minimum_required(VERSION 3.16)
project(BeyondYourComprehensionFHE VERSION 5.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# ═══════════════════════════════════════════
# UNIFIED INCLUDE PATHS
# ═══════════════════════════════════════════
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/seal
    ${CMAKE_SOURCE_DIR}/include/seal/spiral
    ${CMAKE_SOURCE_DIR}/include/engines
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/spiralseal
)

# Find packages
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(OQS REQUIRED liboqs)

# Find SEAL
find_package(SEAL QUIET)
if(SEAL_FOUND)
    add_compile_definitions(HAS_SEAL)
    include_directories(${SEAL_INCLUDE_DIRS})
endif()

# Find OpenFHE
find_path(OPENFHE_INC openfhe/pke/openfhe.h PATHS /usr/local/openfhe/include)
find_library(OPENFHE_PKE OPENFHEpke PATHS /usr/local/openfhe/lib)
find_library(OPENFHE_CORE OPENFHEcore PATHS /usr/local/openfhe/lib)
if(OPENFHE_PKE AND OPENFHE_CORE)
    add_compile_definitions(HAS_OPENFHE)
    include_directories(${OPENFHE_INC}/openfhe/pke ${OPENFHE_INC}/openfhe/core ${OPENFHE_INC}/openfhe/binfhe)
endif()

# ═══════════════════════════════════════════
# EXECUTABLES
# ═══════════════════════════════════════════

# b6_hydra — Main Hydra
add_executable(b6_hydra src/b6_hydra.cpp)
target_link_libraries(b6_hydra OpenSSL::SSL OpenSSL::Crypto ${OQS_LIBRARIES})
if(SEAL_FOUND)
    target_link_libraries(b6_hydra SEAL::seal)
endif()
if(OPENFHE_PKE)
    target_link_libraries(b6_hydra ${OPENFHE_PKE} ${OPENFHE_CORE})
endif()

# test_harmonization_fullblown
add_executable(test_harmonization_fullblown test_harmonization_fullblown.cpp)
target_link_libraries(test_harmonization_fullblown OpenSSL::SSL OpenSSL::Crypto)
if(SEAL_FOUND)
    target_link_libraries(test_harmonization_fullblown SEAL::seal)
endif()

# test_harmonization
add_executable(test_harmonization test_harmonization.cpp)
target_link_libraries(test_harmonization OpenSSL::SSL OpenSSL::Crypto)
if(SEAL_FOUND)
    target_link_libraries(test_harmonization SEAL::seal)
endif()

# test_fullblown_cinematic
add_executable(test_fullblown_cinematic test_fullblown_cinematic.cpp src/fhe/multi_recursive_fhe.cpp)
target_link_libraries(test_fullblown_cinematic OpenSSL::SSL OpenSSL::Crypto)
if(SEAL_FOUND)
    target_link_libraries(test_fullblown_cinematic SEAL::seal)
endif()
