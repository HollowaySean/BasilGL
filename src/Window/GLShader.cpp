#include "GLShader.hpp"

#include <errno.h>
#include <string.h>

#include <fmt/core.h>

#include <fstream>
#include <iostream>

using filepath = std::filesystem::path;

namespace basil {

GLVertexShader::GLVertexShader(filepath path)
    : GLShader::GLShader(path, ShaderType::VERTEX) {}

GLVertexShader::GLVertexShader(const std::string &shaderCode)
    : GLShader::GLShader(shaderCode, ShaderType::VERTEX) {}

std::shared_ptr<GLVertexShader> GLVertexShader::noOpShader() {
    return std::make_shared<GLVertexShader>(NO_OP_VERTEX_CODE);
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

std::shared_ptr<GLFragmentShader> GLFragmentShader::debugShader() {
    return std::make_shared<GLFragmentShader>(DEBUG_FRAGMENT_CODE);
}

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
    hasCompiled = true;
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

        logger.log(
            fmt::format(LOGGER_READ_SUCCESS),
            LogLevel::DEBUG);
        hasCompiled = true;
    } catch(std::ifstream::failure& error) {
        logger.log(
            fmt::format(LOGGER_READ_FAILURE, path.string()),
            LogLevel::ERROR);
        logger.log(
            strerror(errno),
            LogLevel::ERROR);
        hasCompiled = false;
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
    const char* rawShaderCode_cstr = rawShaderCode.c_str();
    glShaderSource(ID, 1, &rawShaderCode_cstr, NULL);
    glCompileShader(ID);
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(ID, 512, NULL, infoLog);
        logger.log(
            fmt::format(LOGGER_COMPILE_FAILURE, ID, typeString),
            LogLevel::ERROR);
        logger.log(infoLog, LogLevel::ERROR);
        hasCompiled = false;
    } else {
        logger.log(
            fmt::format(LOGGER_COMPILE_SUCCESS, ID, typeString),
            LogLevel::INFO);
        hasCompiled &= true;
    }
}

void GLShader::destroyShader() {
    glDeleteShader(ID);
    logger.log(
        fmt::format(LOGGER_DESTROY, ID),
        LogLevel::DEBUG);

    ID = 0;
    hasCompiled = false;
}

GLShader::~GLShader() {
    destroyShader();
}

}  // namespace basil
