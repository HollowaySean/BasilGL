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

void GLShaderProgram::setUniformInt(int value, const std::string& name) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, value);
}

GLShaderProgram::~GLShaderProgram() {
    glDeleteProgram(ID);
}

}  // namespace basil
