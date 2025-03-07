include(FetchContent)

if (NOT TARGET gtest OR NOT TARGET gmock OR NOT TARGET gtest_main)
    message(STATUS "Clarabel.cpp: `googletest` targets not found. Attempting to fetch contents...")
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        origin/main
    )
    FetchContent_MakeAvailable(googletest)
    include(GoogleTest)
    message(STATUS "Clarabel.cpp: `googletest` targets added.")
else()
    message(STATUS "Clarabel.cpp: `googletest` targets found.")
endif()

add_executable(clarabel_cpp_tests
    api_dimension_checks.cpp
    basic_lp.cpp
    basic_qp.cpp
    basic_eq_constrained.cpp
    basic_powcone.cpp
    basic_genpowcone.cpp
    basic_expcone.cpp
    basic_sdp.cpp
    basic_socp.cpp
    basic_unconstrained.cpp
    mixed_conic.cpp
    data_updating.cpp
    sdp_chordal.cpp
    get_info.cpp
)
target_link_libraries(clarabel_cpp_tests 
    libclarabel_c_shared
    Eigen3::Eigen
    GTest::gtest_main
)
gtest_discover_tests(clarabel_cpp_tests)