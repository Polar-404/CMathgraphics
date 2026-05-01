#include <iostream>

#include <fstream>
#include <sstream>

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