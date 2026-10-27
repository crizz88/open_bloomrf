add_executable(open_bloomrf_cli
        src/main.cpp
)

target_link_libraries(open_bloomrf_cli
        PRIVATE
        open_bloomrf
)
if(MSVC)
    target_compile_options(open_bloomrf_cli PRIVATE /arch:AVX2 /O2 /openmp:experimental)
else()
    target_compile_options(open_bloomrf_cli PRIVATE -march=native -O3 -fopenmp-simd)
endif()

target_compile_features(open_bloomrf_cli PRIVATE cxx_std_23)