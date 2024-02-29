#ifndef SRC_WINDOW_GLSHADERPROGRAM_HPP_
#define SRC_WINDOW_GLSHADERPROGRAM_HPP_

#include <GL/glew.h>

#include <memory>
#include <string>

#include "GLShader.hpp"

#include <Basil/Context.hpp>

namespace basil {

/** @brief Container class for compiled and linked shader program. */
class GLShaderProgram : private IBasilContextConsumer {
 public:
    /**
     * @brief Construct a new GLShaderProgram object.
     *
     * @param vertexShader    Const reference to GLVertexShader object.
     * @param fragmentShader  Const reference to GLFragmentShader object.
     */
    GLShaderProgram(
      std::shared_ptr<GLVertexShader> vertexShader,
      std::shared_ptr<GLFragmentShader> fragmentShader);

    /** @brief Deconstructor tears down OpenGL memory usage. */
    ~GLShaderProgram();

    /** @returns  OpenGL-ascribed ID of shader program. */
    GLuint getID() { return ID; }

    /** @brief Calls `glUseProgram` function to activate shader. */
    void use();

    /**
     * @brief Sets a 1D boolean uniform in shader program.
     *
     * @param name   Name of uniform within shader.
     * @param value  Value of uniform.
     */
    void setUniform(const std::string& name, bool value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, uint value);
    void setUniform(const std::string& name, float value);

    template<class T>
    void setUniformVector(const std::string& name,
      T value1, T value2);

    template<class T>
    void setUniformVector(const std::string& name,
      T value1, T value2, T value3);

    template<class T>
    void setUniformVector(const std::string& name,
      T value1, T value2, T value3, T value4);

 private:
    Logger& logger = Logger::get();

    void compile();
    GLuint ID;

    std::shared_ptr<GLVertexShader> vertexShader;
    std::shared_ptr<GLFragmentShader> fragmentShader;
};

}  // namespace basil

#endif  // SRC_WINDOW_GLSHADERPROGRAM_HPP_
