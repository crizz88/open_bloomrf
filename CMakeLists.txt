cmake_minimum_required(VERSION 4.2)
project(open_bloomrf
        VERSION 0.1.0
        DESCRIPTION "bloomRF: point- and range-query Bloom filter"
        LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(core)
add_subdirectory(configurator)


option(BLOOMRF_BUILD_CLI "Build the bloomrf command-line tool" ON)
if(BLOOMRF_BUILD_CLI)
    add_subdirectory(cli)
endif()


include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

option(BLOOMRF_BUILD_TESTS "Build tests" ON)

if(BLOOMRF_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()