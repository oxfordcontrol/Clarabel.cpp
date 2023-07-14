cmake_minimum_required(VERSION 3.13)
project(Clarabel VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build the Rust library

# Define interface libraries for all subdirectories
add_library(libclarabel_c_static INTERFACE)
add_library(libclarabel_c_shared INTERFACE)
# Build Clarabel.rs and link it to the interface libraries
add_subdirectory(rust_wrapper)

# Add other subdirectories
add_subdirectory(examples)