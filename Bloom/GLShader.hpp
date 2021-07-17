#pragma once

#include "Species.cpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLuint GenerateTexture(int text_h, int text_w);
void GenerateVertexBuffer(unsigned int* EBO, unsigned int* VAO, unsigned int* VBO);

//
GLuint LoadShader(const char* fragment_path, const char* vertex_path);
GLuint LoadComputeShader(const char* compute_path);
GLuint LoadEvaporateComputeShader(const char* compute_path);

std::string readFile(const char* filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}


GLuint LoadComputeShader(const char* compute_path)
{
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    std::string computeShaderStr = readFile(compute_path);
    const char* computeShaderSrc = computeShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile compute shader

    std::cout << "Compiling compute shader." << std::endl;
    glShaderSource(computeShader, 1, &computeShaderSrc, NULL);
    glCompileShader(computeShader);

    // Check compute shader

    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &result);
    std::cout << result << std::endl;
    glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> computeShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(computeShader, logLength, NULL, &computeShaderError[0]);
    std::cout << &computeShaderError[0] << std::endl;

    std::cout << "Linking program" << std::endl;
    unsigned int program = glCreateProgram();

    glAttachShader(program, computeShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;
    glDeleteShader(computeShader);
    return program;
}


GLuint LoadEvaporateComputeShader(const char* compute_path)
{
    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    std::string computeShaderStr = readFile(compute_path);
    const char* computeShaderSrc = computeShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile compute shader

    std::cout << "Compiling compute shader." << std::endl;
    glShaderSource(computeShader, 1, &computeShaderSrc, NULL);
    glCompileShader(computeShader);

    // Check compute shader

    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &result);
    std::cout << result << std::endl;
    glGetShaderiv(computeShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> computeShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(computeShader, logLength, NULL, &computeShaderError[0]);
    std::cout << &computeShaderError[0] << std::endl;

    std::cout << "Linking program" << std::endl;
    unsigned int program = glCreateProgram();

    glAttachShader(program, computeShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;
    glDeleteShader(computeShader);
    return program;
}

GLuint LoadShader(const char* fragment_path, const char* vertex_path) {
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

    // Read shaders

    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;


    // Compile vertex shader

    std::cout << "Compiling vertex shader." << std::endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex shader

    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    std::cout << result << std::endl;
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;

    // Compile fragment shader

    std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<GLchar> fragShaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
    for (size_t i = 0; i < fragShaderError[i]; i++)
    {
        std::cout << &fragShaderError[i] << std::endl;
    }

    std::cout << "Linking program" << std::endl;
    unsigned int program = glCreateProgram();

    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}