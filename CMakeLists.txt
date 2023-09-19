cmake_minimum_required(VERSION 3.13)

# Project properties
set(CLARABEL_PROJECT_VERSION 0.5.1)
project(Clarabel VERSION ${CLARABEL_PROJECT_VERSION})

# Specify the default C++ standard applied to all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the default C standard applied to all targets
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# SDP feature configuration
set(CLARABEL_FEATURE_SDP "none" CACHE STRING "Package for SDP to be selected")
set_property(CACHE CLARABEL_FEATURE_SDP PROPERTY STRINGS 
    none
    sdp-accelerate
    sdp-netlib
    sdp-openblas
    sdp-mkl
    sdp-r
)

# Get dependencies

# Eigen3
if (NOT TARGET Eigen3::Eigen)
  find_package(Eigen3 CONFIG REQUIRED)
  message(STATUS "Clarabel.cpp: `Eigen3` package found.")
else()
  message(STATUS "Clarabel.cpp: `Eigen3` package already exists.")
endif()

# Build the Rust library
add_subdirectory(rust_wrapper)

# Add other subdirectories
add_subdirectory(examples)

# Add tests
option(CLARABEL_BUILD_TESTS "Build the unit tests for Clarabel.cpp" false)
if(CLARABEL_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()