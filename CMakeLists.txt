cmake_minimum_required(VERSION 3.16)
project(BeyondYourComprehensionFHE VERSION 6.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Include paths — ALL IN ONE PLACE
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    /usr/local/include/SEAL-4.3
    /usr/local/include/openfhe/pke
    /usr/local/include/openfhe/core
    /usr/local/include/openfhe/binfhe
)

# Find packages
find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(OQS REQUIRED liboqs)

# SEAL
find_package(SEAL QUIET)
if(SEAL_FOUND)
    add_compile_definitions(HAS_SEAL)
endif()

# OpenFHE
find_library(OPENFHE_PKE OPENFHEpke PATHS /usr/local/openfhe/lib)
find_library(OPENFHE_CORE OPENFHEcore PATHS /usr/local/openfhe/lib)
find_library(OPENFHE_BINFHE OPENFHEbinfhe PATHS /usr/local/openfhe/lib)
if(OPENFHE_PKE AND OPENFHE_CORE)
    add_compile_definitions(HAS_OPENFHE)
endif()

# Main executable
add_executable(b6_hydra src/b6_hydra.cpp)
target_link_libraries(b6_hydra 
    OpenSSL::SSL OpenSSL::Crypto 
    ${OQS_LIBRARIES}
    ${OPENFHE_PKE} ${OPENFHE_CORE} ${OPENFHE_BINFHE}
)
if(SEAL_FOUND)
    target_link_libraries(b6_hydra SEAL::seal)
endif()

# Gateway
add_executable(hydra_gateway src/gateway.cpp)
target_link_libraries(hydra_gateway pthread)

# Drogon FHE Gateway
find_package(Drogon REQUIRED)
add_executable(drogon_gateway src/drogon_gateway.cpp)
target_link_libraries(drogon_gateway Drogon::Drogon pthread)
