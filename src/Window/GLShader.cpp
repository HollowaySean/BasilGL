#include "GLShader.hpp"

#include <errno.h>
#include <string.h>

#include <fstream>
#include <iostream>

using filepath = std::filesystem::path;

GLVertexShader::GLVertexShader(filepath path)
    : GLShader::GLShader(path, ShaderType::VERTEX) {}

GLFragmentShader::GLFragmentShader(filepath path)
    : GLShader::GLShader(path, ShaderType::FRAGMENT) {}

GLShader::GLShader(filepath path, ShaderType type) {
    std::string rawShaderCode;
    const char* shaderCode;
    std::ifstream shaderFile;

    // Read shader code from file
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        rawShaderCode = shaderStream.str();
        shaderCode = rawShaderCode.c_str();
    }
    catch(std::ifstream::failure error) {
        // TODO(sholloway): Error handling
        std::cerr << "Error: " << strerror(errno) << std::endl;
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    // Compile the shader
    switch (type) {
        case ShaderType::FRAGMENT:
            ID = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderType::VERTEX:
            ID = glCreateShader(GL_VERTEX_SHADER);
            break;
    }

    GLint success;
    glShaderSource(ID, 1, &shaderCode, NULL);
    glCompileShader(ID);
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        // TODO(sholloway): Error handling
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}
