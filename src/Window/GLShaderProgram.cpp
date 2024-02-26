#include "GLShaderProgram.hpp"

#include <typeinfo>

namespace basil {

GLShaderProgram::GLShaderProgram(
    const GLVertexShader& vertexShader,
    const GLFragmentShader& fragmentShader):
        vertexID(vertexShader.getID()),
        fragmentID(fragmentShader.getID()) {
    this->compile();
}

void GLShaderProgram::compile() {
    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);
    glLinkProgram(ID);

    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, NULL, infoLog);

        logger.log(infoLog, LogLevel::ERROR);
    } else {
        logger.log("Shader program compiled successfully.", LogLevel::INFO);
    }
}

void GLShaderProgram::use() {
    glUseProgram(ID);
}

void GLShaderProgram::setUniform(const std::string& name, bool value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, static_cast<int>(value));
}

void GLShaderProgram::setUniform(const std::string& name, int value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, value);
}

void GLShaderProgram::setUniform(const std::string& name, uint value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1ui(location, value);
}

void GLShaderProgram::setUniform(const std::string& name, float value) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1f(location, value);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform2i(location, static_cast<int>(value1), static_cast<int>(value2));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2, bool value3) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform3i(location, static_cast<int>(value1),
        static_cast<int>(value2), static_cast<int>(value3));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2, bool value3, bool value4) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform4i(location,
        static_cast<int>(value1), static_cast<int>(value2),
        static_cast<int>(value3), static_cast<int>(value4));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform2i(location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2, int value3) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform3i(location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2, int value3, int value4) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform4i(location, value1, value2, value3, value4);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform2ui(location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2, uint value3) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform3ui(location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2, uint value3, uint value4) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform4ui(location, value1, value2, value3, value4);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform2f(location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2, float value3) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform3f(location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2, float value3, float value4) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform4f(location, value1, value2, value3, value4);
}

GLShaderProgram::~GLShaderProgram() {
    glDeleteProgram(ID);
}

}  // namespace basil
