#include <fmt/format.h>

#include "GLShaderProgram.hpp"

#include "Data/ShaderUniformModel.hpp"
#include "File/FileTextureSource.hpp"

namespace basil {

GLShaderProgram::GLShaderProgram(
    std::shared_ptr<GLVertexShader> vertexShader,
    std::shared_ptr<GLFragmentShader> fragmentShader):
        vertexShader(vertexShader),
        fragmentShader(fragmentShader) {
    this->compile();
}

void GLShaderProgram::compile() {
    if (!vertexShader || !fragmentShader) return;

    if (!ID) {
        ID = glCreateProgram();
        attachShader(vertexShader->getID());
        attachShader(fragmentShader->getID());

        uniformManager.setProgramID(ID);
    }

    glLinkProgram(ID);
    int success;
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(ID, 512, NULL, infoLog);

        logger.log(
            fmt::format(LOG_LINK_FAILURE, ID),
            LogLevel::ERROR);
        logger.log(
            infoLog,
            LogLevel::ERROR);

        hasLinked = false;
    } else {
        logger.log(
            fmt::format(LOG_LINK_SUCCESS, ID),
            LogLevel::INFO);
        hasLinked = true;

        // Restore any previously-applied uniforms
        uniformManager.applyCachedUniforms();
    }
}

void GLShaderProgram::use() {
    glUseProgram(ID);
}

void GLShaderProgram::setVertexShader(
        std::shared_ptr<GLVertexShader> setVertexShader) {
    if (!setVertexShader) return;

    if (vertexShader) {
        detachShader(vertexShader->getID());
        attachShader(setVertexShader->getID());
    }

    vertexShader = setVertexShader;
    compile();
}

void GLShaderProgram::setFragmentShader(
        std::shared_ptr<GLFragmentShader> setFragmentShader) {
    if (!setFragmentShader) return;

    if (fragmentShader) {
        detachShader(fragmentShader->getID());
        attachShader(setFragmentShader->getID());
    }

    fragmentShader = setFragmentShader;
    compile();
}

void GLShaderProgram::attachShader(GLint shaderID) {
    glAttachShader(ID, shaderID);
    logger.log(
        fmt::format(LOG_ATTACH, ID, shaderID),
        LogLevel::DEBUG);
}

void GLShaderProgram::detachShader(GLint shaderID) {
    glDetachShader(ID, shaderID);
    logger.log(
        fmt::format(LOG_DETACH, ID, shaderID),
        LogLevel::DEBUG);
}


void GLShaderProgram::receiveData(const DataMessage& message) {
    auto data = message.getData<ShaderUniformModel>();
    if (!data.has_value()) return;
    auto model = data.value();

    auto uniforms = model.getUniforms();
    auto textures = model.getTextures();

    for (auto pair : uniforms) {
        auto uniformID = pair.first;
        auto uniform = pair.second;

        uniformManager.setUniform(uniform);

        // If uniform is attached to a texture, pass into manager
        // to keep pointer to texture alive
        if (textures.contains(uniformID)) {
            auto texture = textures.at(uniformID);
            uniformManager.setTextureSource(texture, uniform);
        }
    }
}

void GLShaderProgram::destroyShaderProgram() {
    glDeleteProgram(ID);

    logger.log(
        fmt::format(LOG_DELETE, ID),
        LogLevel::DEBUG);
    ID = 0;
}

GLShaderProgram::~GLShaderProgram() {
    destroyShaderProgram();
}

GLShaderProgram::Builder&
GLShaderProgram::Builder::withFragmentShader(
        std::shared_ptr<GLFragmentShader> fragmentShader) {
    impl->setFragmentShader(fragmentShader);
    return (*this);
}

GLShaderProgram::Builder&
GLShaderProgram::Builder::withFragmentShaderFromFile(
        std::filesystem::path filePath) {
    impl->setFragmentShader(
        std::make_shared<GLFragmentShader>(filePath));
    return (*this);
}

GLShaderProgram::Builder&
GLShaderProgram::Builder::withFragmentShaderFromCode(
        const std::string& shaderCode) {
    impl->setFragmentShader(
        std::make_shared<GLFragmentShader>(shaderCode));
    return (*this);
}

GLShaderProgram::Builder&
GLShaderProgram::Builder::withVertexShader(
        std::shared_ptr<GLVertexShader> vertexShader) {
    impl->setVertexShader(vertexShader);
    return (*this);
}

GLShaderProgram::Builder&
GLShaderProgram::Builder::withVertexShaderFromFile(
        std::filesystem::path path) {
    impl->setVertexShader(
        std::make_shared<GLVertexShader>(path));
    return (*this);
}

GLShaderProgram::Builder&
GLShaderProgram::Builder::withVertexShaderFromCode(
        const std::string& shaderCode) {
    impl->setVertexShader(
        std::make_shared<GLVertexShader>(shaderCode));
    return (*this);
}

GLShaderProgram::Builder&
GLShaderProgram::Builder::withDefaultVertexShader() {
    impl->setVertexShader(GLVertexShader::noOpShader());
    return (*this);
}

}  // namespace basil
