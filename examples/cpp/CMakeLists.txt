# Define a list of target names
set(CPP_EXAMPLES 
  example_expcone
  example_lp
  example_powcone
  example_powcone_csc
  example_qp
  example_socp
  example_sdp
)

# Define an executable target for each example
foreach(EXAMPLE_NAME ${CPP_EXAMPLES})
  add_executable("cpp_${EXAMPLE_NAME}" ${EXAMPLE_NAME}.cpp)
  target_compile_features("cpp_${EXAMPLE_NAME}" PRIVATE cxx_std_11) # Use C++11

  # Link to clarabel built from the Rust wrapper
  target_link_libraries("cpp_${EXAMPLE_NAME}" PRIVATE libclarabel_c_shared)

  # Link to Eigen
  target_link_libraries("cpp_${EXAMPLE_NAME}" PRIVATE Eigen3::Eigen)
endforeach(EXAMPLE_NAME)
