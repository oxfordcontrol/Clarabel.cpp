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
add_library(utils_c STATIC c/utils.c)
add_dependencies(utils_c clarabel_c)
target_include_directories(utils_c PRIVATE ../include)
target_include_directories(utils_c INTERFACE ./c)
target_link_libraries(utils_c PUBLIC ${clarabel_clib_path})

# Define an executable target for each example
foreach(EXAMPLE_NAME ${C_EXAMPLES})
  add_executable(${EXAMPLE_NAME} c/${EXAMPLE_NAME}.c)
  
  # Use C99
  target_compile_features(${EXAMPLE_NAME} PRIVATE c_std_99)

  # Link to the clarabel library built from the wrapper
  add_dependencies(${EXAMPLE_NAME} clarabel_c)
  target_include_directories(${EXAMPLE_NAME} PRIVATE ../include)
  target_link_libraries(${EXAMPLE_NAME} PUBLIC ${clarabel_clib_path})

  # Link to the utils library
  target_link_libraries(${EXAMPLE_NAME} PRIVATE utils_c)
endforeach(EXAMPLE_NAME)
