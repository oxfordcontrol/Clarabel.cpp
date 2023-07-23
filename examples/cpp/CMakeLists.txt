# Define a list of target names
set(CPP_EXAMPLES 
  # example_expcone
  example_lp
  # example_powcone
  # example_qp
  # example_qp_f32
  # example_qp_f64
  # example_socp
  # example_sdp
)

# Compile utils.c as a library
add_library(utils_cpp INTERFACE)
target_link_libraries(utils_cpp INTERFACE libclarabel_c_shared)
target_include_directories(utils_cpp INTERFACE .) # utils.h
target_include_directories(utils_cpp INTERFACE "${CMAKE_SOURCE_DIR}/include/cpp")

# Define an executable target for each example
foreach(EXAMPLE_NAME ${CPP_EXAMPLES})
  add_executable("cpp_${EXAMPLE_NAME}" ${EXAMPLE_NAME}.cpp)
  target_compile_features("cpp_${EXAMPLE_NAME}" PRIVATE cxx_std_11) # Use C++11

  # Link to the static clarabel library built from the Rust wrapper
  target_link_libraries("cpp_${EXAMPLE_NAME}" PRIVATE libclarabel_c_shared)
  target_include_directories("cpp_${EXAMPLE_NAME}" PRIVATE "${CMAKE_SOURCE_DIR}/include/cpp")

  # Link to the utils library
  target_link_libraries("cpp_${EXAMPLE_NAME}" PRIVATE utils_c)
endforeach(EXAMPLE_NAME)
