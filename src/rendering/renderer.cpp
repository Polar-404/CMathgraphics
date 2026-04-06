#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <fstream>
#include <sstream>

GLFWwindow* window = nullptr;

unsigned int VAO, VBO = 0;
int num_vertices = 0;

std::string read_shader_file(const char* filepath) {
    std::ifstream file(filepath);

    if(!file.is_open()) {
        std::cout << "[ERROR] Failed to open shader file: ' " << filepath << " '" << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void init_opengl() {
    if(!glfwInit()) {
        std::cout << "[ERROR] Failed to init OpenGL." << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "GraphPlotter", NULL, NULL);
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

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void cleanup_opengl() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
    if (window) glfwDestroyWindow(window);
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

    glBindVertexArray(VAO);
    // GL_LINE_STRIP conecta os pontos em sequência, formando o gráfico
    glDrawArrays(GL_LINE_STRIP, 0, num_vertices);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
