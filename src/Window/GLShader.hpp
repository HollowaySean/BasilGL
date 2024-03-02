#ifndef SRC_WINDOW_GLSHADER_HPP_
#define SRC_WINDOW_GLSHADER_HPP_

#include <GL/glew.h>

#include <iostream>
#include <filesystem>
#include <string>

#include <Basil/Context.hpp>
#include <Basil/Logging.hpp>

namespace basil {

/** @brief Container class for OpenGL shader. */
class GLShader : private IBasilContextConsumer {
 public:
    /** @return ID value assigned from OpenGL. */
    virtual GLuint getID() const { return ID; }

    /** @brief Sets shader code from file, and compiles. */
    virtual void setShader(std::filesystem::path path) = 0;

    /** @brief Sets shader code from string, and compiles. */
    virtual void setShader(const std::string& shaderCode) = 0;

    /** @brief Destructor, tears down OpenGL shader code. */
    ~GLShader();

#ifndef TEST_BUILD

 protected:
#endif
    enum ShaderType { FRAGMENT, VERTEX };

    GLShader() = default;
    GLShader(std::filesystem::path path, ShaderType type);
    GLShader(const std::string &shaderCode, ShaderType type);

    void setShaderWithType(std::filesystem::path path, ShaderType type);
    void setShaderWithType(const std::string& shaderCode, ShaderType type);

    void destroyShader();

    GLuint ID = 0;

    static const char* noOpVertexCode;

#ifndef TEST_BUILD

 private:
#endif
    Logger& logger = Logger::get();

    std::string rawShaderCode;
    const char* shaderCode;

    void getShaderFromFile(std::filesystem::path path);
    void getShaderFromString(const std::string &shaderCode);
    void compileShader(ShaderType type);
};

/** @brief GLShader implementation for vertex shader. */
class GLVertexShader : public GLShader {
 public:
    /** @brief Default constructor. Does not compile shader. */
    GLVertexShader() = default;

    /** @brief Pass-through vertex shader. */
    static GLVertexShader noOpShader();

    /** @brief Create vertex shader from file at path. */
    explicit GLVertexShader(std::filesystem::path path);

    /** @brief Create vertex shader from shader code. */
    explicit GLVertexShader(const std::string &shaderCode);

    /** @brief Sets shader code from file, and compiles. */
    void setShader(std::filesystem::path path) override;

    /** @brief Sets shader code from string, and compiles. */
    void setShader(const std::string& shaderCode) override;
};

/** @brief GLShader implementation for fragment shader. */
class GLFragmentShader : public GLShader {
 public:
    /** @brief Default constructor. Does not compile shader. */
    GLFragmentShader() = default;

    /** @brief Create fragment shader from file at path. */
    explicit GLFragmentShader(std::filesystem::path path);

    /** @brief Create fragment shader from shader code. */
    explicit GLFragmentShader(const std::string &shaderCode);

    /** @brief Sets shader code from file, and compiles. */
    void setShader(std::filesystem::path path) override;

    /** @brief Sets shader code from string, and compiles. */
    void setShader(const std::string& shaderCode) override;
};

}  // namespace basil

#endif  // SRC_WINDOW_GLSHADER_HPP_
