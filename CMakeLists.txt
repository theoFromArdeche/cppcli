# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(cppcli)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SRCS
    src/main.cpp
    src/linenoise.c
)

# Executable target
add_executable(cppcli ${SRCS})