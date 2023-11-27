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
    // Read shader code from file
    getShaderFromFile(path);

    // Compile shader code
    compileShader(type);
}

void GLShader::getShaderFromFile(
        std::filesystem::path path) {
    // Read shader code from file
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        rawShaderCode = shaderStream.str();
        shaderCode = rawShaderCode.c_str();

        logger.log("Shader file read successfully.", Level::INFO);
    }
    catch(std::ifstream::failure& error) {
        logger.log("Unable to read shader file.", Level::ERROR);
        logger.log(strerror(errno), Level::ERROR);
    }
}

void GLShader::compileShader(ShaderType type) {
    // Compile the shader
    std::string typeString;
    switch (type) {
        case ShaderType::FRAGMENT:
            ID = glCreateShader(GL_FRAGMENT_SHADER);
            typeString = "fragment";
            break;
        case ShaderType::VERTEX:
            ID = glCreateShader(GL_VERTEX_SHADER);
            typeString = "vertex";
            break;
    }

    GLint success;
    glShaderSource(ID, 1, &shaderCode, NULL);
    glCompileShader(ID);
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        logger.log("Unable to compile " + typeString + " shader.",
            Level::ERROR);
        logger.log(infoLog, Level::ERROR);
    } else {
        logger.log("Shader compiled successfully.", Level::INFO);
    }
}
