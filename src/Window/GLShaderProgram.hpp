#ifndef SRC_WINDOW_GLSHADERPROGRAM_HPP_
#define SRC_WINDOW_GLSHADERPROGRAM_HPP_

#include <GL/glew.h>

#include <memory>
#include <string>

#include "GLShader.hpp"
#include "GLTexture.hpp"

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

    /** @brief Adds reference to texture in shader.
     *
     * @param name    Name of texture within shader.
     * @param texture Pointer to IGLTexture object
     */
    void addTexture(const std::string& name,
      std::shared_ptr<IGLTexture> texture);

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
        /** @brief Add fragment shader object to program. */
        Builder& withFragmentShader(
            std::shared_ptr<GLFragmentShader> fragmentShader);

        /** @brief Build fragment shader from filepath. */
        Builder& withFragmentShaderFromFile(
            std::filesystem::path filePath);

        /** @brief Build fragment shader from raw code. */
        Builder& withFragmentShaderFromCode(
            const std::string& shaderCode);

        /** @brief Add vertex shader object to program. */
        Builder& withVertexShader(
            std::shared_ptr<GLVertexShader> vertexShader);

        /** @brief Build vertex shader from filepath. */
        Builder& withVertexShaderFromFile(
            std::filesystem::path filePath);

        /** @brief Build vertex shader from raw code. */
        Builder& withVertexShaderFromCode(
            const std::string& shaderCode);

        /** @brief Add no-op vertex shader. */
        Builder& withDefaultVertexShader();

        /** @brief Add texture to program. */
        Builder& withTexture(const std::string& name,
          std::shared_ptr<IGLTexture> texture);

        /** @brief Set 1D uniform value. */
        template<class T>
        Builder& withUniform(const std::string& name, T value);

        /** @brief Set 2D uniform value. */
        template<class T>
        Builder& withUniformVector(const std::string& name,
          T value1, T value2);

        /** @brief Set 3D uniform value. */
        template<class T>
        Builder& withUniformVector(const std::string& name,
          T value1, T value2, T value3);


        /** @brief Set 4D uniform value. */
        template<class T>
        Builder& withUniformVector(const std::string& name,
          T value1, T value2, T value3, T value4);
        // TODO(sholloway): withUniform, withTexture
    };

#ifndef TEST_BUILD

 private:
#endif
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
