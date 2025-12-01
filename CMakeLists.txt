cmake_minimum_required(VERSION 3.20)
project(AdventOfCode2025 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(AOC_PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(AdventOfCode2025 src/main.cpp include/common.hpp)

add_subdirectory(days)
target_link_libraries(AdventOfCode2025 PRIVATE aoc_days)
