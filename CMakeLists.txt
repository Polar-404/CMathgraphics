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
    libs/glad/src/glad.c
)

target_include_directories(math_test PRIVATE 
    src 
    libs/glad/include
    libs/glfw-3.4.bin.WIN64/include
)

target_link_directories(math_test PRIVATE 
    libs/glfw-3.4.bin.WIN64/lib-mingw-w64
)

target_link_libraries(math_test glfw3)

if(UNIX AND NOT APPLE)
    target_link_libraries(math_test m)
endif()