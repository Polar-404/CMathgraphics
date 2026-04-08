#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <fstream>
#include <sstream>

#include "utils/readfile.hpp"
#include "drawgrid.hpp"

GLFWwindow* window = nullptr;

unsigned int VAO, VBO = 0;
int num_vertices = 0;

unsigned int shader_program;

const float _WINDOW_WIDTH = 800.0;
const float _WINDOW_HEIGHT = 600.0;


//logs
int success;
char infoLog[512];

void init_opengl() {
    if(!glfwInit()) {
        std::cout << "[ERROR] Failed to init OpenGL." << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(_WINDOW_WIDTH, _WINDOW_HEIGHT, "GraphPlotter", NULL, NULL);
    if(window == NULL) {
        std::cout << "[ERROR] Failed to inicialize a Window for the program." << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "failed to initialize GLAD" << std::endl;
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glLineWidth(2.0f);

    std::string vert_code = read_shader_file("../src/shaders/basic.vert");
    std::string frag_code = read_shader_file("../src/shaders/basic.frag");

    setup_grid_shader();

    const char* vertexShaderSource = vert_code.c_str();
    const char* fragmentShaderSource = frag_code.c_str();

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);
    //ERROR LOGS
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
        std::cout << "[ERROR] Vertex Shader Compilation Failed:\n" << infoLog << std::endl;
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shader);
    //ERROR LOGS
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, infoLog);
        std::cout << "[ERROR] Fragment Shader Compilation Failed:\n" << infoLog << std::endl;
    }

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    //ERROR LOGS
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "[ERROR] Shader Program Linking Failed:\n" << infoLog << std::endl;
    }
    
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void cleanup_opengl() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (window) glfwDestroyWindow(window);
    glDeleteProgram(shader_program);
    glfwTerminate();
}

void update_points(float* data, int count) {
    num_vertices = count / 3;
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}

void render_frame() {
    if (!window || glfwWindowShouldClose(window)) {
        return;
    }
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, 1);
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_grid(_WINDOW_WIDTH, _WINDOW_HEIGHT);

    glUseProgram(shader_program);

    glBindVertexArray(VAO);
    // GL_LINE_STRIP conecta os pontos em sequência, formando o gráfico
    glDrawArrays(GL_LINE_STRIP, 0, num_vertices);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
