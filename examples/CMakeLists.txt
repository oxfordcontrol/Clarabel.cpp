add_executable(c_example_lp c/example_lp.c)
add_dependencies(c_example_lp clarabel_c)
target_include_directories(c_example_lp PRIVATE ../include)
target_link_libraries(c_example_lp PUBLIC ${clarabel_clib_path})