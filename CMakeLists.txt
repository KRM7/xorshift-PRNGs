cmake_minimum_required(VERSION 3.20)

project(xorshift LANGUAGES CXX)

add_executable(${PROJECT_NAME} benchmark.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -DNDEBUG -std=c++20 -Wall -Wextra -pedantic -O2 -flto -fno-unroll-loops -fno-move-loop-invariants)

find_package(benchmark REQUIRED)
target_link_libraries(${PROJECT_NAME} benchmark::benchmark)