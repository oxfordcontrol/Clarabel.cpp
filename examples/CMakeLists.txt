add_executable(example_lp c/example_lp.c)
add_dependencies(example_lp clarabel_c)
target_include_directories(example_lp PRIVATE ../include)
target_link_libraries(example_lp PUBLIC ${clarabel_clib_path})