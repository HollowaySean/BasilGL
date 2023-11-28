#ifndef SRC_WINDOW_GLSHADERPROGRAM_HPP_
#define SRC_WINDOW_GLSHADERPROGRAM_HPP_

#include <GL/glew.h>

#include <string>

#include "GLShader.hpp"

/** @brief Container class for compiled and linked shader program. */
class GLShaderProgram {
 public:
    /**
     * @brief Construct a new GLShaderProgram object.
     *
     * @param vertexShader    Const reference to GLVertexShader object.
     * @param fragmentShader  Const reference to GLFragmentShader object.
     */
    GLShaderProgram(
      const GLVertexShader& vertexShader,
      const GLFragmentShader& fragmentShader);

    /** @returns  OpenGL-ascribed ID of shader program. */
    GLuint getID() { return ID; }

    /** @brief Calls `glUseProgram` function to activate shader. */
    void use();

    /**
     * @brief Sets a 1D integer uniform in shader program.
     *
     * @param value  Integer value of uniform.
     * @param name   Name of uniform within shader.
     */
    void setUniformInt(int value, const std::string& name);

 private:
    Logger& logger = Logger::get();

    void compile();
    GLuint ID;
    GLuint vertexID, fragmentID;
};

#endif  // SRC_WINDOW_GLSHADERPROGRAM_HPP_