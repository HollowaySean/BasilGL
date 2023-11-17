#ifndef SRC_WINDOW_GLSHADER_HPP_
#define SRC_WINDOW_GLSHADER_HPP_

#include <GL/glew.h>

#include <string>
#include <filesystem>

class GLShader {
 public:
    virtual GLuint getID() { return ID; }

 protected:
    enum ShaderType { FRAGMENT, VERTEX };
    GLShader(std::filesystem::path path, ShaderType type);
    GLuint ID;
};

class GLVertexShader : public GLShader {
 public:
    explicit GLVertexShader(std::filesystem::path path);
};

class GLFragmentShader : public GLShader {
 public:
    explicit GLFragmentShader(std::filesystem::path path);
};

#endif  // SRC_WINDOW_GLSHADER_HPP_
