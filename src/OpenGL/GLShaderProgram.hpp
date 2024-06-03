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

#include "GLProgramUniformManager.hpp"
#include "GLShader.hpp"
#include "GLUniform.hpp"

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

    /** @brief Set uniform value in shader. */
    void setUniform(std::shared_ptr<GLUniform> uniform) {
        uniformManager.setUniform(uniform);
    }

    /** @brief Set scalar uniform value in shader. */
    template<class T>
    void setScalarUniform(T value, const std::string& name) {
        setUniform(GLUniformScalar<T>(value, name));
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

        /** @brief Set scalar uniform value. */
        template<class T>
        Builder& withScalarUniform(T value,
                const std::string& name) {
            this->impl->setScalarUniform(value, name);
            return (*this);
        }

        /** @brief Set uniform value. */
        template<class T>
        Builder& withUniform(std::shared_ptr<GLUniform> uniform) {
            this->impl->setUniform(uniform);
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

    GLuint ID = 0;

    GLProgramUniformManager uniformManager;

    std::shared_ptr<GLVertexShader> vertexShader = nullptr;
    std::shared_ptr<GLFragmentShader> fragmentShader = nullptr;

    bool hasLinked = false;

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
