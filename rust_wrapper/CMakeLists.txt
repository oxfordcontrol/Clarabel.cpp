# Build Clarabel.rs (Rust library)
project(clarabel_c VERSION ${CLARABEL_PROJECT_VERSION})

# Define interface targets for the imported Rust library
add_library(libclarabel_c_static INTERFACE)
add_library(libclarabel_c_shared INTERFACE)

# Set the build flags and output directory

# Debug/Release flags
if(CMAKE_BUILD_TYPE MATCHES Release)
   set(clarabel_c_build_flags "--release")
   set(clarabel_c_output_directory "${CMAKE_SOURCE_DIR}/rust_wrapper/target/release")
else()
    set(clarabel_c_build_flags "")
    set(clarabel_c_output_directory "${CMAKE_SOURCE_DIR}/rust_wrapper/target/debug")
endif()

set(CLARABEL_C_OUTPUT_DIR ${clarabel_c_output_directory} PARENT_SCOPE)

# SDP feature flag
if(NOT CLARABEL_FEATURE_SDP STREQUAL "none")
    # Set the Rust feature flag
    set(clarabel_c_build_flags "${clarabel_c_build_flags};--features;${CLARABEL_FEATURE_SDP}")
    
    # Define the FEATURE_SDP flag for all targets that link against clarabel_c
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_SDP)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_SDP)
endif()

# Add the cargo project as a custom target
add_custom_target(
    libclarabel_c
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
    set(LIBCLARABEL_C_SHARED_PATH "${clarabel_c_output_directory}/libclarabel_c.dylib")
    set(LIBCLARABEL_C_STATIC_PATH "${clarabel_c_output_directory}/libclarabel_c.a")
elseif(UNIX)
    set(LIBCLARABEL_C_SHARED_PATH "${clarabel_c_output_directory}/libclarabel_c.so")
    set(LIBCLARABEL_C_STATIC_PATH "${clarabel_c_output_directory}/libclarabel_c.a")
elseif(WIN32)
    set(LIBCLARABEL_C_SHARED_PATH "${clarabel_c_output_directory}/clarabel_c.dll.lib")
    set(LIBCLARABEL_C_STATIC_PATH "${clarabel_c_output_directory}/libclarabel_c.lib")
endif()

# Wrap the Rust library in a CMake library target

# Static library
target_link_libraries(libclarabel_c_static INTERFACE ${LIBCLARABEL_C_STATIC_PATH})
target_include_directories(libclarabel_c_static INTERFACE ${CMAKE_SOURCE_DIR}/include)
add_dependencies(libclarabel_c_static libclarabel_c)

# Shared library
target_link_libraries(libclarabel_c_shared INTERFACE ${LIBCLARABEL_C_SHARED_PATH})
target_include_directories(libclarabel_c_shared INTERFACE ${CMAKE_SOURCE_DIR}/include)
add_dependencies(libclarabel_c_shared libclarabel_c)
