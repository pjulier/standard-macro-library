cmake_minimum_required(VERSION 3.13)
project(SML VERSION 0.2.2 LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build test programs" OFF)

add_library(sml STATIC)
set_property(TARGET sml PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(sml PUBLIC include)
add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)