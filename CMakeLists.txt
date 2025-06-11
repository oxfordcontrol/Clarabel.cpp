cmake_minimum_required(VERSION 3.13)

# Project properties
set(CLARABEL_PROJECT_VERSION 0.10.0)
project(Clarabel VERSION ${CLARABEL_PROJECT_VERSION})

# Specify the default C++ standard applied to all targets
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the default C standard applied to all targets
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Set the root directory of the project. This is needed
# so that Clarabel.cpp can be consumed by other projects via
# `add_subdirectory`
set(CLARABEL_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

#----------------------------------------------
# Clarabel feature configuration
#----------------------------------------------
set(CLARABEL_FEATURE_SDP "none" CACHE STRING "Package for SDP to be selected")
set_property(CACHE CLARABEL_FEATURE_SDP PROPERTY STRINGS
    none
    sdp-accelerate
    sdp-netlib
    sdp-openblas
    sdp-mkl
    sdp-r
)

option(CLARABEL_FEATURE_SERDE "Enable clarabel `serde` option " OFF)
option(CLARABEL_FEATURE_FAER_SPARSE "Enable `faer-sparse` option" OFF)
option(CLARABEL_FEATURE_PARDISO_MKL "Enable `pardiso-mkl` option" OFF)
option(CLARABEL_FEATURE_PARDISO_PANUA "Enable `pardiso-panua` option" OFF)

# option for direct cargo feature specification
set(CLARABEL_CARGO_FEATURES "" CACHE STRING "Comma-separated list of clarabel features for cargo")

#----------------------------------------------
#----------------------------------------------


# Get dependencies

# Eigen3
if (NOT TARGET Eigen3::Eigen)
  find_package(Eigen3 CONFIG REQUIRED VERSION 3.4)
  message(STATUS "Clarabel.cpp: `Eigen3` package found.  Version ${Eigen3_VERSION}.")
else()
  message(STATUS "Clarabel.cpp: `Eigen3` package already exists.  Version ${Eigen3_VERSION}.")
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
