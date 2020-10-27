cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME command_generator)
project(${PROJECT_NAME} CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

## compiler option setting
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

## Use C++17 std
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES "src/*.cc")
add_executable(${PROJECT_NAME} ${SOURCES})