#ifndef SRC_WINDOW_GLSHADERPROGRAM_HPP_
#define SRC_WINDOW_GLSHADERPROGRAM_HPP_

#include <GL/glew.h>

#include <string>

#include "GLShader.hpp"

class GLShaderProgram {
 public:
    GLShaderProgram(GLVertexShader vertexShader, GLFragmentShader fragmentShader);
    GLuint getID() { return ID; }

    void use();

   void setUniformInt(int value, const std::string name);

    template<typename T>
    void setUniform(T value, const std::string name);

 private:
    void compile();
    GLuint ID;
    GLuint vertexID, fragmentID;
};

#endif  // SRC_WINDOW_GLSHADERPROGRAM_HPP_
