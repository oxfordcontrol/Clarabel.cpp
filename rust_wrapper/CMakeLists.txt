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
    set(LIBCLARABEL_C_SHARED_PATH "${clarabel_c_output_directory}/clarabel_c.dll")
    set(LIBCLARABEL_C_STATIC_PATH "${clarabel_c_output_directory}/libclarabel_c.lib")
endif()

# Configure the wrapper targets defined in the root folder

# Static library
target_link_libraries(libclarabel_c_static INTERFACE ${LIBCLARABEL_C_STATIC_PATH})
target_include_directories(libclarabel_c_static INTERFACE ${CMAKE_SOURCE_DIR}/include)
add_dependencies(libclarabel_c_static libclarabel_c)

# Shared library
target_link_libraries(libclarabel_c_shared INTERFACE ${LIBCLARABEL_C_SHARED_PATH})
target_include_directories(libclarabel_c_shared INTERFACE ${CMAKE_SOURCE_DIR}/include)
add_dependencies(libclarabel_c_shared libclarabel_c)
