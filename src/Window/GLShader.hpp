#ifndef SRC_WINDOW_GLSHADER_HPP_
#define SRC_WINDOW_GLSHADER_HPP_

#include <GL/glew.h>

#include <iostream>
#include <filesystem>
#include <string>

#include <Basil/Logging.hpp>

namespace basil {

/** @brief Container class for OpenGL shader. */
class GLShader {
 public:
    /** @return ID value assigned from OpenGL. */
    virtual GLuint getID() const { return ID; }

    /** @brief Destructor, tears down OpenGL shader code. */
    ~GLShader();

#ifndef TEST_BUILD

 protected:
#endif
    enum ShaderType { FRAGMENT, VERTEX };

    GLShader(std::filesystem::path path,
      ShaderType type);

    GLShader(const std::string &shaderCode,
      ShaderType type);

    GLuint ID;

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

    // Unreachable constructor, used for tests
    GLShader() = default;
};

/** @brief GLShader implementation for vertex shader. */
class GLVertexShader : public GLShader {
 public:
    /** @brief Pass-through vertex shader. */
    static GLVertexShader noOpShader();

    /** @brief Create vertex shader from file at path. */
    explicit GLVertexShader(std::filesystem::path path);

#ifndef TEST_BUILD

 private:
#endif
    explicit GLVertexShader(const std::string &shaderCode);
};

/** @brief GLShader implementation for fragment shader. */
class GLFragmentShader : public GLShader {
 public:
    /** @brief Create fragment shader from file at path. */
    explicit GLFragmentShader(std::filesystem::path path);

#ifndef TEST_BUILD

 private:
#endif
    explicit GLFragmentShader(const std::string &shaderCode);
};

}  // namespace basil

#endif  // SRC_WINDOW_GLSHADER_HPP_
