cmake_minimum_required(VERSION 3.10)
project(MathGrapher C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(math_test
    src/main.cpp
    src/calc/tokenize.c
    src/calc/rpn.c
    src/calc/ast.c
    src/rendering/renderer.cpp
)

target_include_directories(math_test PRIVATE src)

if(UNIX AND NOT APPLE)
    target_link_libraries(math_test m)
endif()