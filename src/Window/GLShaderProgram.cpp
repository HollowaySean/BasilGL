#include "GLShaderProgram.hpp"

#include <typeinfo>

GLShaderProgram::GLShaderProgram(GLVertexShader vertexShader, GLFragmentShader fragmentShader) {
    vertexID = vertexShader.getID();
    fragmentID = fragmentShader.getID();

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
        // TODO(sholloway): Error handling
    }
}

void GLShaderProgram::use() {
    glUseProgram(ID);
}

void GLShaderProgram::setUniformInt(int value, const std::string name) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}



template<typename T>
void GLShaderProgram::setUniform(T value, const std::string name) {
    std::type_info type = typeid(T);
    switch (type) {
        case typeid(bool):
            glUniform1i(
                glGetUniformLocation(ID, name.c_str()),
                static_cast<int>(value));
            break;
        case typeid(int):
            glUniform1i(
                glGetUniformLocation(ID, name.c_str()), value);
            break;
        case typeid(float):
            glUniform1f(
                glGetUniformLocation(ID, name.c_str()), value);
            break;
    }
}
