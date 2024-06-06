# Build Clarabel.rs (Rust library)
project(clarabel_c VERSION ${CLARABEL_PROJECT_VERSION})

# Define interface targets for the imported Rust library
add_library(libclarabel_c_static INTERFACE)
add_library(libclarabel_c_shared INTERFACE)

# Set the build flags and output directory

# Debug/Release flags
if(CMAKE_BUILD_TYPE MATCHES Release)
   set(clarabel_c_build_flags "--release")
   set(clarabel_c_output_directory "${CLARABEL_ROOT_DIR}/rust_wrapper/target/release")
else()
    set(clarabel_c_build_flags "")
    set(clarabel_c_output_directory "${CLARABEL_ROOT_DIR}/rust_wrapper/target/debug")
endif()

set(CLARABEL_C_OUTPUT_DIR ${clarabel_c_output_directory} PARENT_SCOPE)


# -------------------------------------
# Cargo features configuration
#--------------------------------------

# A list of features to pass to rustc
set(CLARABEL_BUILD_FEATURES "")

# SDP feature flag
if(NOT CLARABEL_FEATURE_SDP STREQUAL "none")

    # Set the Rust feature flag
    set(CLARABEL_BUILD_FEATURES "${CLARABEL_BUILD_FEATURES},${CLARABEL_FEATURE_SDP}")

    # Define the FEATURE_SDP flag for all targets that link against clarabel_c
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_SDP)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_SDP)
endif()

# FAER_SPARSE feature flag
if(CLARABEL_FEATURE_FAER_SPARSE)

    # Set the Rust feature flag
    set(CLARABEL_BUILD_FEATURES "${CLARABEL_BUILD_FEATURES},faer-sparse")

    # Define the FEATURE_FAER_SPARSE flag for all targets that link against clarabel_c
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_FAER_SPARSE)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_FAER_SPARSE)
endif()

# SERDE feature flag
if(CLARABEL_FEATURE_SERDE)

    # Set the Rust feature flag
    set(CLARABEL_BUILD_FEATURES "${CLARABEL_BUILD_FEATURES},serde")

    # Define the FEATURE_SERDE flag for all targets that link against clarabel_c
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_SERDE)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_SERDE)
endif()



set(clarabel_c_build_flags "${clarabel_c_build_flags};--features=${CLARABEL_BUILD_FEATURES}")
message("-- Rust feature list: " ${CLARABEL_BUILD_FEATURES})
message("-- Cargo options: " "${clarabel_c_build_flags}")


# -------------------------------------
# -------------------------------------


# Get the path to the Rust library for linking
if(APPLE)
    set(LIBCLARABEL_C_SHARED_PATH "${clarabel_c_output_directory}/libclarabel_c.dylib")
    set(LIBCLARABEL_C_STATIC_PATH "${clarabel_c_output_directory}/libclarabel_c.a")
elseif(UNIX)
    set(LIBCLARABEL_C_SHARED_PATH "${clarabel_c_output_directory}/libclarabel_c.so")
    set(LIBCLARABEL_C_STATIC_PATH "${clarabel_c_output_directory}/libclarabel_c.a")
elseif(WIN32)
    set(LIBCLARABEL_C_SHARED_PATH "${clarabel_c_output_directory}/clarabel_c.dll.lib")
    set(LIBCLARABEL_C_STATIC_PATH "${clarabel_c_output_directory}/clarabel_c.lib")
endif()


# Add the cargo project as a custom target
add_custom_target(
    libclarabel_c
    WORKING_DIRECTORY ${CLARABEL_ROOT_DIR}/rust_wrapper
    # Commands for building the Rust library
    COMMAND cargo build ${clarabel_c_build_flags}
    COMMAND cargo install cbindgen --version 0.24.5
    # Generate the C header
    COMMAND cbindgen --config cbindgen.toml --crate clarabel_c --output ./headers/clarabel.h --lang c
    # Generate the C++ header
    COMMAND cbindgen --config cbindgen.toml --crate clarabel_c --output ./headers/clarabel.hpp
    BYPRODUCTS
        "${LIBCLARABEL_C_SHARED_PATH}"
        "${LIBCLARABEL_C_STATIC_PATH}"
)


# Wrap the Rust library in a CMake library target

# Static library
target_link_libraries(libclarabel_c_static INTERFACE ${LIBCLARABEL_C_STATIC_PATH})
target_include_directories(libclarabel_c_static INTERFACE ${CLARABEL_ROOT_DIR}/include)
add_dependencies(libclarabel_c_static libclarabel_c)

# Shared library
target_link_libraries(libclarabel_c_shared INTERFACE ${LIBCLARABEL_C_SHARED_PATH})
target_include_directories(libclarabel_c_shared INTERFACE ${CLARABEL_ROOT_DIR}/include)
add_dependencies(libclarabel_c_shared libclarabel_c)
