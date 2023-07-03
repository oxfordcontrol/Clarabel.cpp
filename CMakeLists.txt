cmake_minimum_required(VERSION 3.10)
project(Clarabel VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build the Rust library
#if(CMAKE_BUILD_TYPE MATCHES Release)
#    set(clarabel_c_flags "--release")
#endif()

add_custom_target(
    clarabel_c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/rust_wrapper
    # Commands for building the Rust library
    COMMAND cargo build
    COMMAND cargo install cbindgen --version 0.24.5
    # Generate the C header
    COMMAND cbindgen --config cbindgen.toml --crate clarabel_c --output ../include/clarabel.h --lang c
    # Generate the C++ header
    COMMAND cbindgen --config cbindgen.toml --crate clarabel_c --output ../include/clarabel.hpp
)

# Get the path to the Rust library for linking
if(APPLE)
    set(clarabel_clib_path "${CMAKE_SOURCE_DIR}/rust_wrapper/target/debug/libclarabel_c.dylib")
elseif(UNIX)
    set(clarabel_clib_path "${CMAKE_SOURCE_DIR}/rust_wrapper/target/debug/libclarabel_c.so")
elseif(WIN32)
    set(clarabel_clib_path "${CMAKE_SOURCE_DIR}/rust_wrapper/target/debug/clarabel_c.dll")
endif()

# Add subdirectories
add_subdirectory(examples)