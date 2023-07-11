# Define a list of target names
set(C_EXAMPLES example_expcone example_lp example_powcone)

# Define an executable target for each example
foreach(EXAMPLE_NAME ${C_EXAMPLES})
  add_executable(${EXAMPLE_NAME} c/${EXAMPLE_NAME}.c)
  
  # Use C99
  target_compile_features(${EXAMPLE_NAME} PRIVATE c_std_99)

  # Link to the clarabel library built from the wrapper
  add_dependencies(${EXAMPLE_NAME} clarabel_c)
  target_include_directories(${EXAMPLE_NAME} PRIVATE ../include)
  target_link_libraries(${EXAMPLE_NAME} PUBLIC ${clarabel_clib_path})
endforeach(EXAMPLE_NAME)
