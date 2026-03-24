# CMakeLists.txt for ARM NEON Optimization

cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject LANGUAGES C CXX)

# Specify the architecture for ARM NEON
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon")

# Add your source files here
add_executable(MyProject main.cpp)