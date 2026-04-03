set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.10)
project(MathGrapher C)

set(CMAKE_C_STANDARD 11)

add_executable(math_test main.c tokenize.c)

if(UNIX AND NOT APPLE)
    target_link_libraries(math_test m)
endif()