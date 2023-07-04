cmake_minimum_required(VERSION 3.10)
project(Clarabel VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build Clarabel.rs (Rust library)

# Set the build flags and output directory
if(CMAKE_BUILD_TYPE MATCHES Release)
   set(clarabel_c_build_flags "--release")
   set(clarabel_c_output_directory "${CMAKE_SOURCE_DIR}/rust_wrapper/target/release/")
else()
    set(clarabel_c_build_flags "")
    set(clarabel_c_output_directory "${CMAKE_SOURCE_DIR}/rust_wrapper/target/debug/")
endif()

# Add the cargo project as a custom target
add_custom_target(
    clarabel_c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/rust_wrapper
    # Commands for building the Rust library
    COMMAND cargo build ${clarabel_c_build_flags}
    COMMAND cargo install cbindgen --version 0.24.5
    # Generate the C header
    COMMAND cbindgen --config cbindgen.toml --crate clarabel_c --output ./headers/clarabel.h --lang c
    # Generate the C++ header
    COMMAND cbindgen --config cbindgen.toml --crate clarabel_c --output ./headers/clarabel.hpp
)

# Get the path to the Rust library for linking
if(APPLE)
    set(clarabel_clib_path "${clarabel_c_output_directory}/libclarabel_c.dylib")
elseif(UNIX)
    set(clarabel_clib_path "${clarabel_c_output_directory}/libclarabel_c.so")
elseif(WIN32)
    set(clarabel_clib_path "${clarabel_c_output_directory}/clarabel_c.dll")
endif()

# Add subdirectories
add_subdirectory(examples)