# Define a list of target names
set(CPP_EXAMPLES
  example_expcone
  example_lp
  example_powcone
  example_genpowcone
  example_qp
  example_socp
  example_sdp
  example_json
  example_print_stream
  example_callback
    example_callback_with_state
  example_faer
  example_pardiso_mkl
)

# Define an executable target for each example
foreach(EXAMPLE_NAME ${CPP_EXAMPLES})
  add_executable("cpp_${EXAMPLE_NAME}" ${EXAMPLE_NAME}.cpp)
  target_compile_features("cpp_${EXAMPLE_NAME}" PRIVATE cxx_std_14) # Eigen requires at least c++14 support

  # Link to clarabel built from the Rust wrapper
  target_link_libraries("cpp_${EXAMPLE_NAME}" PRIVATE libclarabel_c_shared)
  # On Windows, copy the shared library and debug symbols to the output directory for targets that link to it
  if(WIN32)
    add_custom_command(
        TARGET "cpp_${EXAMPLE_NAME}"
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CLARABEL_C_OUTPUT_DIR}/clarabel_c.dll
        "$<TARGET_FILE_DIR:cpp_${EXAMPLE_NAME}>"
    )
    add_custom_command(
        TARGET "cpp_${EXAMPLE_NAME}"
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CLARABEL_C_OUTPUT_DIR}/clarabel_c.pdb
        "$<TARGET_FILE_DIR:cpp_${EXAMPLE_NAME}>"
    )
  endif()

  # Link to Eigen
  target_link_libraries("cpp_${EXAMPLE_NAME}" PRIVATE Eigen3::Eigen)
endforeach(EXAMPLE_NAME)
