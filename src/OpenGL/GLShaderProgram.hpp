#pragma once

#include <GL/glew.h>

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/Context.hpp>
#include <Basil/Packages/PubSub.hpp>

#include "OpenGL/GLUniform.hpp"

#include "GLShader.hpp"
#include "GLTexture.hpp"

namespace basil {

/** @brief Container class for compiled and linked shader program. */
class GLShaderProgram : public IDataSubscriber,
                        public IBuildable<GLShaderProgram>,
                        private IBasilContextConsumer {
 public:
    /** @brief Construct a new GLShaderProgram object. */
    GLShaderProgram() = default;

    /** @brief Construct a new GLShaderProgram object.
     *
     * @param vertexShader    Shared_ptr to GLVertexShader object.
     * @param fragmentShader  Shared_ptr to GLFragmentShader object. */
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

    /** @returns GLVertexShader object. */
    std::shared_ptr<GLVertexShader> getVertexShader() {
        return vertexShader;
    }

    /** @brief Set GLFragmentShader object. */
    void setFragmentShader(
        std::shared_ptr<GLFragmentShader> fragmentShader);

    /** @returns GLFragmentShader object. */
    std::shared_ptr<GLFragmentShader> getFragmentShader() {
        return fragmentShader;
    }

    /** @returns Boolean indicating linking success. */
    bool hasLinkedSuccessfully() { return hasLinked; }

    /** @brief Adds reference to texture in shader.
     *
     *  @param name    Name of texture within shader.
     *  @param texture Pointer to IGLTexture object */
    void addTexture(const std::string& name,
        std::shared_ptr<IGLTexture> texture);

    /** @brief Sets a 1D uniform in shader program.
     *
     *  @param name   Name of uniform within shader.
     *  @param value  Value of uniform. */
    template<class T>
    void setUniform(const std::string& name, T value)  {
        cacheUniform(name, std::vector({ value }));

        GLint location = getUniformLocation(name);
        if (location == -1) return;

        setUniformAt(location, value);
    }

    /** @brief Sets a 2D uniform in shader program.
     *
     *  @param name   Name of uniform within shader.
     *  @param value1 x-value
     *  @param value2 y-value */
    template<class T>
    void setUniform(const std::string& name,
            T value1, T value2)  {
        cacheUniform(name, std::vector({ value1, value2 }));

        GLint location = getUniformLocation(name);
        if (location == -1) return;

        setUniformAt(location, value1, value2);
    }

    /** @brief Sets a 3D uniform in shader program.
     *
     *  @param name   Name of uniform within shader.
     *  @param value1 x-value
     *  @param value2 y-value
     *  @param value3 z-value */
    template<class T>
    void setUniform(const std::string& name,
            T value1, T value2, T value3)  {
        cacheUniform(name, std::vector({ value1, value2, value3 }));

        GLint location = getUniformLocation(name);
        if (location == -1) return;

        setUniformAt(location, value1, value2, value3);
    }

    /** @brief Sets a 4D uniform in shader program.
     *
     *  @param name   Name of uniform within shader.
     *  @param value1 x-value
     *  @param value2 y-value
     *  @param value3 z-value
     *  @param value4 w-value */
    template<class T>
    void setUniform(const std::string& name,
            T value1, T value2, T value3, T value4)  {
        cacheUniform(name, std::vector({ value1, value2, value3, value4 }));

        GLint location = getUniformLocation(name);
        if (location == -1) return;

        setUniformAt(location, value1, value2, value3, value4);
    }

    /** @brief Sets a uniform of 1-4 dimensions in shader program.
     *
     *  @param name   Name of uniform within shader.
     *  @param values Vector of values. */
    template<class T>
    void setUniformVector(const std::string& name, std::vector<T> values) {
        GLint location = getUniformLocation(name);
        if (location == -1) return;

        cacheUniform(name, static_cast<GLUniformVector>(values));
        setUniformVectorAt(location, values);
    }

    /** @brief Updates shaders and textures from ShaderUniformModel object.
     *  Overridden method from IDataSubscriber base class. */
    void receiveData(const DataMessage& message) override;

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
        Builder& withUniform(const std::string& name,
                T value1, T value2) {
            this->impl->setUniform(name, value1, value2);
            return (*this);
        }

        /** @brief Set 3D uniform value. */
        template<class T>
        Builder& withUniform(const std::string& name,
                T value1, T value2, T value3) {
            this->impl->setUniform(name, value1, value2, value3);
            return (*this);
        }

        /** @brief Set 4D uniform value. */
        template<class T>
        Builder& withUniform(const std::string& name,
                T value1, T value2, T value3, T value4) {
            this->impl->setUniform(name, value1, value2, value3, value4);
            return (*this);
        }

        /** @brief Set 1D-4D uniform value. */
        template<class T>
        Builder& withUniformVector(const std::string& name,
                std::vector<T> values) {
            this->impl->setUniformVector(name, values);
            return (*this);
        }
    };

#ifndef TEST_BUILD

 private:
#endif
    Logger& logger = Logger::get();

    void compile();

    void attachShader(GLint shaderID);
    void detachShader(GLint shaderID);

    void destroyShaderProgram();

    GLint getUniformLocation(const std::string& name);

    template<class T>
    void setUniformAt(GLint location, T value);
    template<class T>
    void setUniformAt(GLint location, T value1, T value2);
    template<class T>
    void setUniformAt(GLint location, T value1, T value2, T value3);
    template<class T>
    void setUniformAt(GLint location, T value1, T value2, T value3, T value4);
    template<class T>
    void setUniformVectorAt(GLint location, std::vector<T> values) {
        switch (values.size()) {
            case 1:
                setUniformAt<T>(location, values[0]);
                break;
            case 2:
                setUniformAt<T>(
                    location, values[0], values[1]);
                break;
            case 3:
                setUniformAt<T>(
                    location, values[0], values[1], values[2]);
                break;
            default:
                setUniformAt<T>(
                    location, values[0], values[1], values[2], values[3]);
        }
  }

    void visitUniform(const std::string& name, GLUniformScalar value);
    void visitUniform(const std::string& name, GLUniformVector value);

    void cacheUniform(const std::string& name, GLUniformVector values);
    void applyCachedUniforms();
    std::map<std::string, GLUniformVector> uniformCache;
    std::set<std::string> errorHistory;

    GLuint ID = 0;

    std::shared_ptr<GLVertexShader> vertexShader = nullptr;
    std::shared_ptr<GLFragmentShader> fragmentShader = nullptr;

    bool hasLinked = false;

    std::vector<std::shared_ptr<IGLTexture>> textures;

    LOGGER_FORMAT LOG_LINK_SUCCESS =
        "Shader Program (ID{:02}) - Program linked successfully.";
    LOGGER_FORMAT LOG_LINK_FAILURE =
        "Shader Program (ID{:02}) - Failed to link program. "
        "See OpenGL info log:";

    LOGGER_FORMAT LOG_ATTACH =
        "Shader Program (ID{:02}) - Attaching shader (ID{:02}).";
    LOGGER_FORMAT LOG_DETACH =
        "Shader Program (ID{:02}) - Detaching shader (ID{:02}).";

    LOGGER_FORMAT LOG_DELETE =
        "Shader Program (ID{:02}) - Program deleted.";

    LOGGER_FORMAT LOG_UNIFORM_FAILURE =
        "Shader Program (ID{:02}) - Could not get location for uniform "
        "with name \"{}\".";
};

}   // namespace basil
