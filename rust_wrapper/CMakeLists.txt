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

if(NOT DEFINED CLARABEL_RUST_FEATURES)
    set(CLARABEL_RUST_FEATURES "")
endif()

# -------------------------------------
# Cargo feature configuration helper functions
#--------------------------------------

# Check if CLARABEL_RUST_FEATURES contains a specific feature
function(rust_feature_is_enabled PATTERN RESULT_VAR)
    string(REPLACE "," ";" FEATURE_LIST "${CLARABEL_RUST_FEATURES}")
    set(${RESULT_VAR} FALSE PARENT_SCOPE)
    foreach(FEATURE ${FEATURE_LIST})
        if(FEATURE MATCHES "${PATTERN}")
            set(${RESULT_VAR} TRUE PARENT_SCOPE)
            break()
        endif()
    endforeach()
endfunction()

# Check if CLARABEL_RUST_FEATURES contains any of the "sdp" options
function(rust_features_has_sdp RESULT_VAR)
    if(CLARABEL_RUST_FEATURES MATCHES ".*sdp.*")
        set(${RESULT_VAR} TRUE PARENT_SCOPE)
    else()
        set(${RESULT_VAR} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Helper function to append a feature to a comma-separated list
function(append_feature FEATURE_LIST_VAR NEW_FEATURE)
    if(${FEATURE_LIST_VAR} STREQUAL "")
        set(${FEATURE_LIST_VAR} "${NEW_FEATURE}" PARENT_SCOPE)
    else()
        set(${FEATURE_LIST_VAR} "${${FEATURE_LIST_VAR}},${NEW_FEATURE}" PARENT_SCOPE)
    endif()
endfunction()

# Automatically prefix features with 'clarabel/' if not already prefixed
# For serde, add it as both clarabel/serde and serde, with the latter 
# necessary for the rust_wrapper cargo configuration.

function(format_clarabel_rust_features INPUT_FEATURES OUTPUT_VAR)
    string(REPLACE "," ";" FEATURE_LIST "${INPUT_FEATURES}")
    set(FORMATTED_FEATURES "")
    foreach(FEATURE ${FEATURE_LIST})
        # Regular handling: prefix with clarabel/ if not already prefixed
        if(NOT FEATURE MATCHES "^clarabel/")
            set(FEATURE "clarabel/${FEATURE}")
        endif()
        append_feature(FORMATTED_FEATURES "${FEATURE}")
    endforeach()
    set(${OUTPUT_VAR} "${FORMATTED_FEATURES}" PARENT_SCOPE)
endfunction()


# -------------------------------------
# Cargo feature configuration backward compatibility
#--------------------------------------


if(NOT CLARABEL_FEATURE_SDP STREQUAL "none")
    append_feature(CLARABEL_RUST_FEATURES "${CLARABEL_FEATURE_SDP}")
endif()

if(CLARABEL_FEATURE_FAER_SPARSE)
    append_feature(CLARABEL_RUST_FEATURES "faer-sparse")
endif()

if(CLARABEL_FEATURE_PARDISO_MKL)
    append_feature(CLARABEL_RUST_FEATURES "pardiso-mkl")
endif()

# SERDE feature flag
if(CLARABEL_FEATURE_SERDE)
    append_feature(CLARABEL_RUST_FEATURES "serde")
endif()


# -------------------------------------
# Cargo feature configuration 
#--------------------------------------

# ----
# Rust features should be passed via the CLARABEL_RUST_FEATURES variable.
# For some features, we requires some additional CMake configuration.
# --- 

rust_features_has_sdp(CONFIG_SDP)
if(CONFIG_SDP)
    # Define the FEATURE_SDP flag for all targets that link against clarabel_c
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_SDP)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_SDP)
endif()

rust_feature_is_enabled("faer-sparse" CONFIG_FAER_SPARSE)
if(CONFIG_FAER_SPARSE)
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_FAER_SPARSE)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_FAER_SPARSE)
endif()

rust_feature_is_enabled("pardiso-mkl" CONFIG_PARDISO_MKL)
if(CONFIG_PARDISO_MKL)
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_PARDISO_MKL)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_PARDISO_MKL)
endif()

rust_feature_is_enabled("pardiso-panua" CONFIG_PARDISO_PANUA)
if(CONFIG_PARDISO_PANUA)
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_PARDISO_PANUA)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_PARDISO_PANUA)
endif()

if(CONFIG_PARDISO_MKL OR CONFIG_PARDISO_PANUA)
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_PARDISO_ANY)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_PARDISO_ANY)
endif()

rust_feature_is_enabled("serde" CONFIG_SERDE)
if(CONFIG_SERDE)
    target_compile_definitions(libclarabel_c_static INTERFACE FEATURE_SERDE)
    target_compile_definitions(libclarabel_c_shared INTERFACE FEATURE_SERDE)
endif()

# replace each rust <feature> with clarabel/<feature> so that they pass through 
# this wrapper to the clarabel crate underneath
format_clarabel_rust_features("${CLARABEL_RUST_FEATURES}" CLARABEL_RUST_FEATURES)

# some features must also be specified at the wrapper level 
# add them again, but after prepending clarabel/ to everything
if(CONFIG_SDP)
    append_feature(CLARABEL_RUST_FEATURES "sdp")
endif()
if(CONFIG_SERDE)
    append_feature(CLARABEL_RUST_FEATURES "serde")
endif()

if(NOT CLARABEL_RUST_FEATURES STREQUAL "")
    set(clarabel_c_build_flags "${clarabel_c_build_flags};--features=${CLARABEL_RUST_FEATURES}")
endif()

message("-- Rust feature list: " ${CLARABEL_RUST_FEATURES})
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
    COMMAND cargo install cbindgen
    # Generate the C header
    COMMAND cbindgen --config cbindgen.toml --quiet --crate clarabel_c --output ./headers/clarabel.h --lang c
    # Generate the C++ header
    COMMAND cbindgen --config cbindgen.toml --quiet --crate clarabel_c --output ./headers/clarabel.hpp
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
