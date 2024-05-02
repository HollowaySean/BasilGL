#include "GLShaderProgram.hpp"

#include <typeinfo>

namespace basil {

GLShaderProgram::GLShaderProgram(
    std::shared_ptr<GLVertexShader> vertexShader,
    std::shared_ptr<GLFragmentShader> fragmentShader):
        vertexShader(vertexShader),
        fragmentShader(fragmentShader) {
    this->updateShaders();
}

void GLShaderProgram::compile() {
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader->getID());
    glAttachShader(ID, fragmentShader->getID());
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

void GLShaderProgram::updateShaders() {
    destroyShaderProgram();

    if (vertexShader && fragmentShader) {
        compile();
    }
}

void GLShaderProgram::use() {
    glUseProgram(ID);
}

void GLShaderProgram::setVertexShader(
        std::shared_ptr<GLVertexShader> vertexShader) {
    this->vertexShader = vertexShader;
    updateShaders();
}

void GLShaderProgram::setFragmentShader(
        std::shared_ptr<GLFragmentShader> fragmentShader) {
    this->fragmentShader = fragmentShader;
    updateShaders();
}

void GLShaderProgram::addTexture(const std::string& name,
        std::shared_ptr<IGLTexture> texture) {
    use();
    GLint location =
        glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, texture->getUniformLocation());

    // Add texture to vector to prevent it from falling out of scope
    textures.push_back(texture);
}

void GLShaderProgram::setUniform(const std::string& name, bool value) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, static_cast<int>(value));
}

void GLShaderProgram::setUniform(const std::string& name, int value) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1i(location, value);
}

void GLShaderProgram::setUniform(const std::string& name, uint value) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1ui(location, value);
}

void GLShaderProgram::setUniform(const std::string& name, float value) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform1f(location, value);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform2i(location, static_cast<int>(value1), static_cast<int>(value2));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2, bool value3) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform3i(location, static_cast<int>(value1),
        static_cast<int>(value2), static_cast<int>(value3));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        bool value1, bool value2, bool value3, bool value4) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform4i(location,
        static_cast<int>(value1), static_cast<int>(value2),
        static_cast<int>(value3), static_cast<int>(value4));
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform2i(location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2, int value3) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform3i(location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        int value1, int value2, int value3, int value4) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform4i(location, value1, value2, value3, value4);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform2ui(location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2, uint value3) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform3ui(location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        uint value1, uint value2, uint value3, uint value4) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform4ui(location, value1, value2, value3, value4);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform2f(location, value1, value2);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2, float value3) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform3f(location, value1, value2, value3);
}

template<>
void GLShaderProgram::setUniformVector(const std::string& name,
        float value1, float value2, float value3, float value4) {
    use();
    GLint location = glGetUniformLocation(ID, name.c_str());
    glUniform4f(location, value1, value2, value3, value4);
}

void GLShaderProgram::applyDataModel(const ShaderUniformModel& dataModel) {
    auto uniforms = dataModel.getUniforms();

    for (auto uniform : uniforms) {
        const std::string& name = uniform.name;

        std::visit([&](const auto& value){ setUniform(name, value); },
            uniform.value);
    }
}

void GLShaderProgram::destroyShaderProgram() {
    glDeleteProgram(ID);
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
    impl->setVertexShader(
        std::make_shared<GLVertexShader>(GLVertexShader::noOpShader()));
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
