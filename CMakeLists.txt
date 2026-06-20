cmake_minimum_required(VERSION 3.16)
project(BeyondYourComprehensionFHE VERSION 5.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_DISABLE_FIND_PACKAGE_SEAL ON)
set(CMAKE_DISABLE_FIND_PACKAGE_OpenFHE ON)

find_package(OpenSSL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(OQS REQUIRED liboqs)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(b6_hydra src/b6_hydra.cpp)
target_link_libraries(b6_hydra OpenSSL::SSL OpenSSL::Crypto ${OQS_LIBRARIES})
