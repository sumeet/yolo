cmake_minimum_required(VERSION 3.0)
project(yolo C)

set(CMAKE_C_STANDARD 11)

add_executable(yolo main.c parser.h parser.c)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Wconversion")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
