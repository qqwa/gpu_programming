cmake_minimum_required(VERSION 3.14)
project(gpu_programming)

set(CMAKE_CXX_STANDARD 14)

add_executable(gpu_programming src/main.cpp src/utils.cpp src/utils.h)
target_link_libraries(gpu_programming OpenCL)