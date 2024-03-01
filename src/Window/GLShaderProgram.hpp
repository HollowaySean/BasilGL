#ifndef SRC_WINDOW_GLSHADERPROGRAM_HPP_
#define SRC_WINDOW_GLSHADERPROGRAM_HPP_

#include <GL/glew.h>

#include <memory>
#include <string>

#include "GLShader.hpp"

#include <Basil/Builder.hpp>
#include <Basil/Context.hpp>

namespace basil {

/** @brief Container class for compiled and linked shader program. */
class GLShaderProgram : public IBuildable<GLShaderProgram>,
                        private IBasilContextConsumer {
 public:
    /** @brief Construct a new GLShaderProgram object. */
    GLShaderProgram() = default;

    /**
     * @brief Construct a new GLShaderProgram object.
     *
     * @param vertexShader    Shared_ptr to GLVertexShader object.
     * @param fragmentShader  Shared_ptr to GLFragmentShader object.
     */
    GLShaderProgram(
      std::shared_ptr<GLVertexShader> vertexShader,
      std::shared_ptr<GLFragmentShader> fragmentShader);

    /** @brief Deconstructor tears down OpenGL memory usage. */
    ~GLShaderProgram();

    /** @brief Calls `glUseProgram` function to activate shader. */
    void use();

    /** @returns  OpenGL-ascribed ID of shader program. */
    GLuint getID() { return ID; }

    /** @brief Set GLVertexShader object. */
    void setVertexShader(
      std::shared_ptr<GLVertexShader> vertexShader);

    /** @brief Set GLFragmentShader object. */
    void setFragmentShader(
      std::shared_ptr<GLFragmentShader> fragmentShader);

    /**
     * @brief Sets a 1D boolean uniform in shader program.
     *
     * @param name   Name of uniform within shader.
     * @param value  Value of uniform.
     */
    void setUniform(const std::string& name, bool value);

    /**
     * @brief Sets a 1D integer uniform in shader program.
     *
     * @param name   Name of uniform within shader.
     * @param value  Value of uniform.
     */
    void setUniform(const std::string& name, int value);

    /**
     * @brief Sets a 1D unsigned int uniform in shader program.
     *
     * @param name   Name of uniform within shader.
     * @param value  Value of uniform.
     */
    void setUniform(const std::string& name, uint value);

    /**
     * @brief Sets a 1D float uniform in shader program.
     *
     * @param name   Name of uniform within shader.
     * @param value  Value of uniform.
     */
    void setUniform(const std::string& name, float value);

    /**
     * @brief Sets a 2D uniform in shader program.
     *
     * @param name   Name of uniform within shader.
     * @param value  Value of uniform.
     */
    template<class T>
    void setUniformVector(const std::string& name,
      T value1, T value2);

    /**
     * @brief Sets a 3D uniform in shader program.
     *
     * @param name   Name of uniform within shader.
     * @param value  Value of uniform.
     */

    template<class T>
    void setUniformVector(const std::string& name,
      T value1, T value2, T value3);

    /**
     * @brief Sets a 4D uniform in shader program.
     *
     * @param name   Name of uniform within shader.
     * @param value  Value of uniform.
     */

    template<class T>
    void setUniformVector(const std::string& name,
      T value1, T value2, T value3, T value4);

    class Builder : public IBuilder<GLShaderProgram> {
     public:
        Builder& withFragmentShader(std::filesystem::path filePath);
        Builder& withVertexShader(std::filesystem::path filePath);
        Builder& withDefaultVertexShader();
    };

 private:
    Logger& logger = Logger::get();

    void compile();
    void updateShaders();

    void destroyShaderProgram();

    GLuint ID = 0;

    std::shared_ptr<GLVertexShader> vertexShader = nullptr;
    std::shared_ptr<GLFragmentShader> fragmentShader = nullptr;
};

}  // namespace basil

#endif  // SRC_WINDOW_GLSHADERPROGRAM_HPP_
