add_library(open_bloomrf STATIC
        src/bloomrf.cpp
        include/open_bloomrf/config.hpp
        include/open_bloomrf/dyadic/trace_level.hpp
        include/open_bloomrf/dyadic/decomposition.hpp
        include/open_bloomrf/dyadic/interval.hpp
        src/dyadic/decomposition.cpp
        src/dyadic/trace_level.cpp
        include/open_bloomrf/bloomrf.hpp
        include/open_bloomrf/storage/storage_layout.hpp
        include/open_bloomrf/dyadic/dyadic_trace_tree.hpp
        src/storage/storage_layout.cpp
        include/open_bloomrf/storage/bit_array.hpp
        include/open_bloomrf/storage/cache_line_block.hpp
)

target_include_directories(open_bloomrf
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if(MSVC)
    target_compile_options(open_bloomrf PRIVATE /arch:AVX2 /O2 /openmp:experimental)
else()
    target_compile_options(open_bloomrf PRIVATE -march=native -O3 -fopenmp-simd)
endif()

target_compile_features(open_bloomrf PUBLIC cxx_std_23)

target_compile_options(open_bloomrf PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)