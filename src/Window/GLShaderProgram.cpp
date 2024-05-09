#include "GLShaderProgram.hpp"

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
        applyChachedUniforms();
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

GLint GLShaderProgram::getUniformLocation(const std::string& name) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        logger.log(
            fmt::format(LOG_UNIFORM_FAILURE, ID, name),
            LogLevel::DEBUG);
    }
    return location;
}

template<>
void GLShaderProgram::setUniformAt<bool>(GLint location, bool value) {
    glProgramUniform1i(ID, location, static_cast<int>(value));
}

template<>
void GLShaderProgram::setUniformAt<int>(GLint location, int value) {
    glProgramUniform1i(ID, location, value);
}

template<>
void GLShaderProgram::setUniformAt<uint>(GLint location, uint value) {
    glProgramUniform1ui(ID, location, value);
}

template<>
void GLShaderProgram::setUniformAt<float>(GLint location, float value) {
    glProgramUniform1f(ID, location, value);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        bool value1, bool value2) {
    glProgramUniform2i(ID, location,
        static_cast<int>(value1), static_cast<int>(value2));
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        bool value1, bool value2, bool value3) {
    glProgramUniform3i(ID, location, static_cast<int>(value1),
        static_cast<int>(value2), static_cast<int>(value3));
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        bool value1, bool value2, bool value3, bool value4) {
    glProgramUniform4i(ID, location,
        static_cast<int>(value1), static_cast<int>(value2),
        static_cast<int>(value3), static_cast<int>(value4));
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        int value1, int value2) {
    glProgramUniform2i(ID, location, value1, value2);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        int value1, int value2, int value3) {
    glProgramUniform3i(ID, location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        int value1, int value2, int value3, int value4) {
    glProgramUniform4i(ID, location, value1, value2, value3, value4);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        uint value1, uint value2) {
    glProgramUniform2ui(ID, location, value1, value2);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        uint value1, uint value2, uint value3) {
    glProgramUniform3ui(ID, location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        uint value1, uint value2, uint value3, uint value4) {
    glProgramUniform4ui(ID, location, value1, value2, value3, value4);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        float value1, float value2) {
    glProgramUniform2f(ID, location, value1, value2);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        float value1, float value2, float value3) {
    glProgramUniform3f(ID, location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformAt(GLint location,
        float value1, float value2, float value3, float value4) {
    glProgramUniform4f(ID, location, value1, value2, value3, value4);
}

void GLShaderProgram::addTexture(const std::string& name,
        std::shared_ptr<IGLTexture> texture) {
    int textureLocation = texture->getUniformLocation();
    cacheUniform(name, std::vector({ textureLocation }));

    GLint location = getUniformLocation(name);
    if (location == -1) return;

    setUniformAt(location, textureLocation);

    // Add texture to vector to prevent it from falling out of scope
    textures.push_back(texture);
}

void GLShaderProgram::cacheUniform(
        const std::string& name, GLUniformVector values) {
    if (uniformCache.contains(name)) {
        uniformCache.at(name) = values;
    } else {
        uniformCache.emplace(name, values);
    }
}

void GLShaderProgram::applyChachedUniforms() {
    for (auto uniformRecord : uniformCache) {
        visitUniform(uniformRecord.first, uniformRecord.second);
    }
}

void GLShaderProgram::receiveData(const DataMessage& message) {
    if (!std::holds_alternative<ShaderUniformModel>(message.model)) return;

    auto model = std::get<ShaderUniformModel>(message.model);

    auto uniforms = model.getUniforms();
    for (auto pair : uniforms) {
        auto uniform = pair.second;
        const std::string& name = uniform.name;

        std::visit([&](const auto& value){
                visitUniform(name, value);
            }, uniform.value);
    }

    auto modelTextures = model.getTextures();
    for (auto pair : modelTextures) {
        auto texture = pair.second;
        addTexture(texture.name, texture.texture);
    }
}

void GLShaderProgram::visitUniform(
        const std::string& name, GLUniformScalar value) {
    std::visit([&](const auto& value){ setUniform(name, value); }, value);
}

void GLShaderProgram::visitUniform(
        const std::string& name, GLUniformVector value) {
    std::visit([&](const auto& value){ setUniformVector(name, value); }, value);
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

GLShaderProgram::Builder&
GLShaderProgram::Builder::withTexture(const std::string& name,
        std::shared_ptr<IGLTexture> texture) {
    impl->addTexture(name, texture);
    return (*this);
}

template<>
GLShaderProgram::Builder&
GLShaderProgram::Builder::withUniform(const std::string& name, bool value) {
    impl->setUniform(name, value);
    return (*this);
}

template<>
GLShaderProgram::Builder&
GLShaderProgram::Builder::withUniform(const std::string& name, int value) {
    impl->setUniform(name, value);
    return (*this);
}

template<>
GLShaderProgram::Builder&
GLShaderProgram::Builder::withUniform(const std::string& name, uint value) {
    impl->setUniform(name, value);
    return (*this);
}

template<>
GLShaderProgram::Builder&
GLShaderProgram::Builder::withUniform(const std::string& name, float value) {
    impl->setUniform(name, value);
    return (*this);
}

}  // namespace basil
