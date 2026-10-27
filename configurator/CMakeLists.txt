add_library(bloomrf_configurator STATIC
        src/planner.cpp
        src/fpr_model.cpp
)

target_include_directories(bloomrf_configurator
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(bloomrf_configurator
        PUBLIC
        bloomrf
)

target_compile_features(bloomrf_configurator PUBLIC cxx_std_23)

target_compile_options(bloomrf_configurator PRIVATE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
)