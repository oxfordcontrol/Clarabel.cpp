cmake_minimum_required(VERSION 3.13)

# Project properties
set(CLARABEL_PROJECT_VERSION 0.1)
project(Clarabel VERSION ${CLARABEL_PROJECT_VERSION})

# Specify the default C++ standard applied to all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the default C standard applied to all targets
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)


# Build the Rust library
add_subdirectory(rust_wrapper)

# Add other subdirectories
add_subdirectory(examples)