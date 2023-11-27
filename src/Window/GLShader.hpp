#ifndef SRC_WINDOW_GLSHADER_HPP_
#define SRC_WINDOW_GLSHADER_HPP_

#include <GL/glew.h>

#include <iostream>
#include <filesystem>
#include <string>

#include "Logger.hpp"

/** @brief Container class for OpenGL shader. */
class GLShader {
 public:
    /** @return ID value assigned from OpenGL. */
    virtual GLuint getID() const { return ID; }

#ifndef TEST_BUILD

 protected:
#endif
    enum ShaderType { FRAGMENT, VERTEX };

    GLShader(std::filesystem::path path,
      ShaderType type);

    GLuint ID;

#ifndef TEST_BUILD

 private:
#endif
    Logger& logger = Logger::get();

    std::string rawShaderCode;
    const char* shaderCode;

    void getShaderFromFile(
      std::filesystem::path path);

    void compileShader(
      ShaderType type);

    // Unreachable constructor, used for tests
    GLShader() = default;
};

/** @brief GLShader implementation for vertex shader. */
class GLVertexShader : public GLShader {
 public:
    explicit GLVertexShader(std::filesystem::path path);
};

/** @brief GLShader implementation for fragment shader. */
class GLFragmentShader : public GLShader {
 public:
    explicit GLFragmentShader(std::filesystem::path path);
};

#endif  // SRC_WINDOW_GLSHADER_HPP_
