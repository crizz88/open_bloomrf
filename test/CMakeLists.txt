add_executable(open_bloomrf_tests
        trace_tree_test.cpp
        dtt_storage_layout_test.cpp
        dyadic_trace_tree_test.cpp
        storage_test.cpp
)

target_link_libraries(open_bloomrf_tests
        PRIVATE
        open_bloomrf
        GTest::gtest
        GTest::gtest_main
)

if(MSVC)
    target_compile_options(open_bloomrf_tests PRIVATE /arch:AVX2 /O2 /openmp:experimental)
else()
    target_compile_options(open_bloomrf_tests PRIVATE -march=native -O3 -fopenmp-simd)
endif()

include(GoogleTest)
gtest_discover_tests(open_bloomrf_tests)