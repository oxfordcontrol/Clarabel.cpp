project(clarabel_examples VERSION ${CLARABEL_PROJECT_VERSION})
add_compile_definitions("EXAMPLES_ROOT_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(c)
add_subdirectory(cpp)