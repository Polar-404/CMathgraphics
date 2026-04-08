#include <glad/glad.h>
#include "utils/readfile.hpp"

#include <iostream>

unsigned int gridVAO, gridVBO, gridShader;

void 
setup_grid_shader() {

    std::string vert_code = read_shader_file("../src/shaders/grid.vert");
    std::string frag_code = read_shader_file("../src/shaders/grid.frag");

    const char* vertexShaderSource = vert_code.c_str();
    const char* fragmentShaderSource = frag_code.c_str();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shader);

    gridShader = glCreateProgram();
    glAttachShader(gridShader, vertex_shader);
    glAttachShader(gridShader, fragment_shader);
    glLinkProgram(gridShader);

    float vertices[] = { //space where the frag shader will run (in this case, the entire screen (-1,-1 up to 1,1))
        -1.0f, 1.0f,  0.0f,
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f,  0.0f,
        1.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);

    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
}

void 
draw_grid(float w_width, float w_height) {
    glUseProgram(gridShader);

    glUniform1f(glGetUniformLocation(gridShader, "WINDOW_WIDTH"), w_width);
    glUniform1f(glGetUniformLocation(gridShader, "WINDOW_HEIGHT"), w_height);

    glBindVertexArray(gridVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}