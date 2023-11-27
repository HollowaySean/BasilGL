#include "GLShaderProgram.hpp"

#include <typeinfo>

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

        logger.log(infoLog, Level::ERROR);
    } else {
        logger.log("Shader program compiled successfully.", Level::INFO);
    }
}

void GLShaderProgram::use() {
    glUseProgram(ID);
}

void GLShaderProgram::setUniformInt(int value, const std::string& name) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
