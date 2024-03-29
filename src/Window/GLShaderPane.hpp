#ifndef SRC_WINDOW_GLSHADERPANE_HPP_
#define SRC_WINDOW_GLSHADERPANE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "GLTexture.hpp"
#include "GLShaderProgram.hpp"
#include "IPane.hpp"

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
     * @param paneProps     Pane size properties
     * @param shaderProgram GLShaderProgram object to render
    */
    GLShaderPane(PaneProps paneProps,
      std::shared_ptr<GLShaderProgram> shaderProgram)
        : shaderProgram(shaderProgram),
          IPane(paneProps) {
            setup();
          }

    /** @brief Creates blank GLShaderPane. */
    GLShaderPane() = default;

    /** @brief Destructor unbinds OpenGL-allocated memory. */
    ~GLShaderPane();

    /** @brief Set shader program in use. */
    void setShaderProgram(std::shared_ptr<GLShaderProgram> shaderProgram);

    /** @param newTexture Texture to pass to shader in each draw call. */
    void addTexture(std::shared_ptr<IGLTexture> newTexture);

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

        /** @brief Uses texture in shader program. */
        Builder& withTexture(
            std::shared_ptr<IGLTexture> texture);
    };

#ifndef TEST_BUILD

 private:
#endif
    void setup();
    void createVertexObjects();
    void createElementBuffer();

    GLuint vertexAttributeID = 0;
    GLuint vertexBufferID = 0;
    GLuint elementBufferID = 0;
    std::vector<std::shared_ptr<IGLTexture>> textureList;
    std::shared_ptr<GLShaderProgram> shaderProgram = nullptr;
};

}  // namespace basil

#endif  // SRC_WINDOW_GLSHADERPANE_HPP_
