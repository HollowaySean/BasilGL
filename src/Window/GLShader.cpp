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

GLShader::GLShader(filepath path, ShaderType type, std::ostream& ostream) {
    // Read shader code from file
    getShaderFromFile(path, ostream);

    // Compile shader code
    compileShader(type, ostream);
}

void GLShader::getShaderFromFile(
        std::filesystem::path path, std::ostream& ostream) {
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

        logger.log("Shader file read successfully.", Level::INFO, ostream);
    }
    catch(std::ifstream::failure& error) {
        logger.log("Unable to read shader file.", Level::ERROR, ostream);
        logger.log(strerror(errno), Level::ERROR, ostream);
    }
}

void GLShader::compileShader(ShaderType type, std::ostream& ostream) {
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
            Level::ERROR, ostream);
        logger.log(infoLog, Level::ERROR, ostream);
    } else {
        logger.log("Shader compiled successfully.", Level::INFO, ostream);
    }
}
