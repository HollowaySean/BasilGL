#include "GLShader.hpp"

#include <errno.h>
#include <string.h>

#include <fstream>
#include <iostream>

using filepath = std::filesystem::path;

namespace basil {

const char* GLShader::noOpVertexCode =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "void main() {\n"
    "gl_Position = vec4(aPos, 1.0);\n"
    "TexCoord = aTexCoord; }\0";

GLVertexShader::GLVertexShader(filepath path)
    : GLShader::GLShader(path, ShaderType::VERTEX) {}

GLVertexShader::GLVertexShader(const std::string &shaderCode)
    : GLShader::GLShader(shaderCode, ShaderType::VERTEX) {}

GLVertexShader GLVertexShader::noOpShader() {
    return GLVertexShader(
        std::string(GLShader::noOpVertexCode));
}

void GLVertexShader::setShader(filepath path) {
    setShaderWithType(path, ShaderType::VERTEX);
}

void GLVertexShader::setShader(const std::string &shaderCode) {
    setShaderWithType(shaderCode, ShaderType::VERTEX);
}


GLFragmentShader::GLFragmentShader(filepath path)
    : GLShader::GLShader(path, ShaderType::FRAGMENT) {}

GLFragmentShader::GLFragmentShader(const std::string &shaderCode)
    : GLShader::GLShader(shaderCode, ShaderType::FRAGMENT) {}

void GLFragmentShader::setShader(filepath path) {
    setShaderWithType(path, ShaderType::FRAGMENT);
}

void GLFragmentShader::setShader(const std::string &shaderCode) {
    setShaderWithType(shaderCode, ShaderType::FRAGMENT);
}


GLShader::GLShader(filepath path, ShaderType type) {
    getShaderFromFile(path);
    compileShader(type);
}

GLShader::GLShader(const std::string &shaderCode, ShaderType type) {
    getShaderFromString(shaderCode);
    compileShader(type);
}

void GLShader::setShaderWithType(filepath path, ShaderType type) {
    destroyShader();

    getShaderFromFile(path);
    compileShader(type);
}

void GLShader::setShaderWithType(
        const std::string &shaderCode, ShaderType type) {
    destroyShader();

    getShaderFromString(shaderCode);
    compileShader(type);
}

void GLShader::getShaderFromString(const std::string &shaderCode) {
    this->rawShaderCode = shaderCode;
    this->shaderCode = rawShaderCode.c_str();
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

        logger.log("Shader file read successfully.", LogLevel::INFO);
    }
    catch(std::ifstream::failure& error) {
        logger.log("Unable to read shader file.", LogLevel::ERROR);
        logger.log(strerror(errno), LogLevel::ERROR);
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
            LogLevel::ERROR);
        logger.log(infoLog, LogLevel::ERROR);
    } else {
        logger.log("Shader compiled successfully.", LogLevel::INFO);
    }
}

void GLShader::destroyShader() {
    glDeleteShader(ID);
}

GLShader::~GLShader() {
    destroyShader();
}

}  // namespace basil
