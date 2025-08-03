cmake_minimum_required(VERSION 3.13)

project(SML VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(sml STATIC)
set_property(TARGET sml PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(sml PUBLIC include)
add_subdirectory(src)

add_subdirectory(test)