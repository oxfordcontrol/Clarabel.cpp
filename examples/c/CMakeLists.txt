# Define a list of target names
set(C_EXAMPLES 
  example_expcone
  example_lp
  example_powcone
  example_qp
  example_qp_f32
  example_socp
)

# Compile utils.c as a library
add_library(utils_c STATIC utils.c)
target_link_libraries(utils_c PUBLIC libclarabel_c_shared)

# Define an executable target for each example
foreach(EXAMPLE_NAME ${C_EXAMPLES})
  add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}.c)
  
  # Use C11
  target_compile_features(${EXAMPLE_NAME} PRIVATE c_std_11)

  # Link to the static clarabel library built from the Rust wrapper
  target_link_libraries(${EXAMPLE_NAME} PRIVATE libclarabel_c_shared)

  # Link to the utils library
  target_link_libraries(${EXAMPLE_NAME} PRIVATE utils_c)
endforeach(EXAMPLE_NAME)
