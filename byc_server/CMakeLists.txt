cmake_minimum_required(VERSION 3.5)
project(BYC_FHE_Server)

find_package(Drogon REQUIRED)
find_package(Jsoncpp REQUIRED)

add_executable(byc_server main.cpp)
target_link_libraries(byc_server Drogon::Drogon Jsoncpp_lib)
