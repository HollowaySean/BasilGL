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
            fmt::format(LOGGER_LINK_FAILURE, ID),
            LogLevel::INFO);
        logger.log(
            infoLog,
            LogLevel::ERROR);
    } else {
        logger.log(
            fmt::format(LOGGER_LINK_SUCCESS, ID),
            LogLevel::INFO);
    }
}

void GLShaderProgram::use() {
    glUseProgram(ID);
}

void GLShaderProgram::setVertexShader(
        std::shared_ptr<GLVertexShader> setVertexShader) {
    if (vertexShader) {
        detachShader(vertexShader->getID());
        attachShader(setVertexShader->getID());
    }

    vertexShader = setVertexShader;
    compile();
}

void GLShaderProgram::setFragmentShader(
        std::shared_ptr<GLFragmentShader> setFragmentShader) {
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
        fmt::format(LOGGER_ATTACH, ID, shaderID),
        LogLevel::DEBUG);
}

void GLShaderProgram::detachShader(GLint shaderID) {
    glDetachShader(ID, shaderID);
    logger.log(
        fmt::format(LOGGER_DETACH, ID, shaderID),
        LogLevel::DEBUG);
}

void GLShaderProgram::addTexture(const std::string& name,
        std::shared_ptr<IGLTexture> texture) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform1i(ID, location, texture->getUniformLocation());

    // Add texture to vector to prevent it from falling out of scope
    textures.push_back(texture);
}

GLint GLShaderProgram::getUniformLocation(const std::string& name) {
    GLint location = glGetUniformLocation(ID, name.c_str());
    if (location == -1) {
        logger.log(
            fmt::format(LOGGER_UNIFORM_FAILURE, ID, name),
            LogLevel::DEBUG);
    }
    return location;
}

template<>
void GLShaderProgram::setUniform<bool>(const std::string& name, bool value) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform1i(ID, location, static_cast<int>(value));
}

template<>
void GLShaderProgram::setUniform<int>(const std::string& name, int value) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform1i(ID, location, value);
}

template<>
void GLShaderProgram::setUniform<uint>(const std::string& name, uint value) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform1ui(ID, location, value);
}

template<>
void GLShaderProgram::setUniform<float>(const std::string& name, float value) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform1f(ID, location, value);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform2i(ID, location,
        static_cast<int>(value1), static_cast<int>(value2));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2, bool value3) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform3i(ID, location, static_cast<int>(value1),
        static_cast<int>(value2), static_cast<int>(value3));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2, bool value3, bool value4) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform4i(ID, location,
        static_cast<int>(value1), static_cast<int>(value2),
        static_cast<int>(value3), static_cast<int>(value4));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform2i(ID, location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2, int value3) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform3i(ID, location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2, int value3, int value4) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform4i(ID, location, value1, value2, value3, value4);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform2ui(ID, location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2, uint value3) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform3ui(ID, location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2, uint value3, uint value4) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform4ui(ID, location, value1, value2, value3, value4);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform2f(ID, location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2, float value3) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform3f(ID, location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2, float value3, float value4) {
    GLint location = getUniformLocation(name);
    if (location == -1) return;

    glProgramUniform4f(ID, location, value1, value2, value3, value4);
}

template<class T>
void GLShaderProgram::setUniformVector(
        const std::string& name, std::vector<T> values) {
    switch (values.size()) {
        case 1:
            setUniform<T>(name, values[0]);
            break;
        case 2:
            setUniformVector<T>(
                name, values[0], values[1]);
            break;
        case 3:
            setUniformVector<T>(
                name, values[0], values[1], values[2]);
            break;
        default:
            setUniformVector<T>(
                name, values[0], values[1], values[2], values[3]);
    }
}

void GLShaderProgram::receiveData(const ShaderUniformModel& dataModel) {
    auto uniforms = dataModel.getUniforms();
    for (auto pair : uniforms) {
        auto uniform = pair.second;
        const std::string& name = uniform.name;

        std::visit([&](const auto& value){
                visitUniform(name, value);
            }, uniform.value);
    }

    auto modelTextures = dataModel.getTextures();
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
        fmt::format(LOGGER_DELETE, ID),
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
