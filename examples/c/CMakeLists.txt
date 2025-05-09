# Define a list of target names
set(C_EXAMPLES 
  example_expcone
  example_lp
  example_powcone
  example_genpowcone
  example_qp
  example_qp_f32
  example_qp_f64
  example_socp
  example_sdp
  example_json
  example_print_stream
  example_callback
  example_callback_with_state
  example_pardiso_mkl
)

# Compile utils.c as a library
add_library(utils_c STATIC utils.c)
target_link_libraries(utils_c PUBLIC libclarabel_c_shared)
target_include_directories(utils_c INTERFACE .) # utils.h

# Define an executable target for each example
foreach(EXAMPLE_NAME ${C_EXAMPLES})
  add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}.c)
  target_compile_features(${EXAMPLE_NAME} PRIVATE c_std_11) # Use C11

  # Link to clarabel built from the Rust wrapper
  target_link_libraries(${EXAMPLE_NAME} PRIVATE libclarabel_c_shared)
  # On Windows, copy the shared library and debug symbols to the output directory for targets that link to it
  if(WIN32)
    add_custom_command(
        TARGET ${EXAMPLE_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CLARABEL_C_OUTPUT_DIR}/clarabel_c.dll
        "$<TARGET_FILE_DIR:${EXAMPLE_NAME}>"
    )
    add_custom_command(
        TARGET ${EXAMPLE_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CLARABEL_C_OUTPUT_DIR}/clarabel_c.pdb
        "$<TARGET_FILE_DIR:${EXAMPLE_NAME}>"
    )
  endif()

  # Link to the utils library
  target_link_libraries(${EXAMPLE_NAME} PRIVATE utils_c)
endforeach(EXAMPLE_NAME)

