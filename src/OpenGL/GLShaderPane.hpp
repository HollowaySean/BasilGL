#pragma once

#include <filesystem>
#include <memory>
#include <string>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/Context.hpp>

#include "Window/IPane.hpp"

#include "GLShader.hpp"
#include "GLShaderProgram.hpp"

namespace basil {

/**
 * @brief Pane which renders to screen at a given position,
 *  using shaders and textures provided.
*/
class GLShaderPane :  public IPane,
                      public IBuildable<GLShaderPane>,
                      private IBasilContextConsumer {
 public:
    /**
     * @brief Creates GLShaderPane from existing GLShaderProgram.
     *
     * @param viewArea     Pane size properties
     * @param shaderProgram GLShaderProgram object to render
    */
    GLShaderPane(ViewArea viewArea,
        std::shared_ptr<GLShaderProgram> shaderProgram)
          : IPane(viewArea) {
      setShaderProgram(shaderProgram);
    }

    /** @brief Creates blank GLShaderPane. */
    GLShaderPane() = default;

    /** @brief Destructor unbinds OpenGL-allocated memory. */
    ~GLShaderPane();

    /** @brief Set shader program in use. */
    void setShaderProgram(std::shared_ptr<GLShaderProgram> shaderProgram);

    /** @brief Draws to screen using shader and texture(s). */
    void const draw() override;

    class Builder : public IBuilder<GLShaderPane> {
     public:
        /** @brief Creates pane from GLFragmentShader object. */
        Builder& fromShader(
            std::shared_ptr<GLFragmentShader> fragmentShader);

        /** @brief Creates pane from fragment shader file. */
        Builder& fromShaderFile(
            std::filesystem::path filePath);

        /** @brief Creates pane from raw fragment shader code. */
        Builder& fromShaderCode(
            const std::string& shaderCode);

        /** @brief Creates pane from GLShaderProgram object. */
        Builder& withShaderProgram(
          std::shared_ptr<GLShaderProgram> shaderProgram);
    };

#ifndef TEST_BUILD

 protected:
#endif
    void setupGLBuffers();
    void createVertexObjects();
    void createElementBuffer();

    GLuint vertexAttributeID = 0;
    GLuint vertexBufferID = 0;
    GLuint elementBufferID = 0;
    std::shared_ptr<GLShaderProgram> currentShaderProgram = nullptr;
};

}   // namespace basil
