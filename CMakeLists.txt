cmake_minimum_required(VERSION 3.16)
project(BeyondYourComprehensionFHE VERSION 6.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# ═══════════════════════════════════════════
# ALL ENGINES — PERMANENTLY ACTIVE
# ═══════════════════════════════════════════
add_compile_definitions(HAS_SEAL)
add_compile_definitions(HAS_OPENFHE)
add_compile_definitions(HAS_TFHE)
add_compile_definitions(HAS_ZAMA)

# Include paths
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/spiralseal
    /usr/local/include/SEAL-4.3
    /usr/local/openfhe/include
    /usr/local/openfhe/include/openfhe/pke
    /usr/local/openfhe/include/openfhe/core
    /usr/local/openfhe/include/openfhe/binfhe
)

# Link directories
link_directories(
    /usr/local/openfhe/lib
    /usr/local/lib
)

# Find packages
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(OQS REQUIRED liboqs)

# SEAL
find_package(SEAL QUIET)
if(SEAL_FOUND)
    include_directories(${SEAL_INCLUDE_DIRS})
endif()

# OpenFHE — FORCE FIND WITH EXACT PATHS
set(OPENFHE_INC /usr/local/openfhe/include)
set(OPENFHE_PKE /usr/local/openfhe/lib/libOPENFHEpke.so)
set(OPENFHE_CORE /usr/local/openfhe/lib/libOPENFHEcore.so)
set(OPENFHE_BINFHE /usr/local/openfhe/lib/libOPENFHEbinfhe.so)

# ═══════════════════════════════════════════
# EXECUTABLES — ALL ENGINES LINKED
# ═══════════════════════════════════════════

# b6_hydra — ALL 4 ENGINES
add_executable(b6_hydra src/b6_hydra.cpp)
target_link_libraries(b6_hydra 
    OpenSSL::SSL OpenSSL::Crypto 
    ${OQS_LIBRARIES}
    ${OPENFHE_PKE}
    ${OPENFHE_CORE}
    ${OPENFHE_BINFHE}
)
if(SEAL_FOUND)
    target_link_libraries(b6_hydra SEAL::seal)
endif()

# test_harmonization
add_executable(test_harmonization test_harmonization.cpp)
target_link_libraries(test_harmonization 
    OpenSSL::SSL OpenSSL::Crypto
    ${OPENFHE_PKE} ${OPENFHE_CORE}
)
if(SEAL_FOUND)
    target_link_libraries(test_harmonization SEAL::seal)
endif()

# test_harmonization_fullblown
add_executable(test_harmonization_fullblown test_harmonization_fullblown.cpp)
target_link_libraries(test_harmonization_fullblown 
    OpenSSL::SSL OpenSSL::Crypto
    ${OPENFHE_PKE} ${OPENFHE_CORE}
)
if(SEAL_FOUND)
    target_link_libraries(test_harmonization_fullblown SEAL::seal)
endif()
