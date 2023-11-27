#ifndef SRC_WINDOW_GLSHADERPROGRAM_HPP_
#define SRC_WINDOW_GLSHADERPROGRAM_HPP_

#include <GL/glew.h>

#include <string>

#include "GLShader.hpp"

class GLShaderProgram {
 public:
    GLShaderProgram(
      const GLVertexShader& vertexShader,
      const GLFragmentShader& fragmentShader);
    GLuint getID() { return ID; }

    void use();

    void setUniformInt(int value, const std::string& name);

 private:
    Logger& logger = Logger::get();

    void compile();
    GLuint ID;
    GLuint vertexID, fragmentID;
};

#endif  // SRC_WINDOW_GLSHADERPROGRAM_HPP_
