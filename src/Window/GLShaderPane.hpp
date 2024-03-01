#ifndef SRC_WINDOW_GLSHADERPANE_HPP_
#define SRC_WINDOW_GLSHADERPANE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

#include "GLTexture.hpp"
#include "GLShaderProgram.hpp"
#include "IPane.hpp"

namespace basil {

/**
 * @class GLShaderPane
 * @brief Pane which renders to screen at a given position,
 *  using shaders and textures provided.
*/
class GLShaderPane :  public IPane,
                      private IBasilContextConsumer {
 public:
    /**
     * @brief Creates GLShaderPane from existing GLShaderProgram.
     *
     * @param paneProps     Pane size properties
     * @param shaderProgram GLShaderProgram object to render
    */
    explicit GLShaderPane(
      PaneProps paneProps,
      std::shared_ptr<GLShaderProgram> shaderProgram)
        : vertexAttributeID(),
          vertexBufferID(),
          elementBufferID(),
          shaderProgram(shaderProgram),
          IPane(paneProps) {
            setup();
          }

    /** @brief Destructor unbinds OpenGL-allocated memory. */
    ~GLShaderPane();

    /** @param newTexture Texture to pass to shader in each draw call. */
    void addTexture(std::shared_ptr<IGLTexture> newTexture);

    /** @brief Draws to screen using shader and texture(s). */
    void const draw() override;

#ifndef TEST_BUILD

 private:
#endif
    void setup();
    void createVertexObjects();
    void createElementBuffer();

    GLuint vertexAttributeID, vertexBufferID, elementBufferID;
    std::vector<std::shared_ptr<IGLTexture>> textureList;
    std::shared_ptr<GLShaderProgram> shaderProgram;
};

}  // namespace basil

#endif  // SRC_WINDOW_GLSHADERPANE_HPP_
