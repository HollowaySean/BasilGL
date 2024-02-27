#ifndef SRC_WINDOW_GLTEXTUREPANE_HPP_
#define SRC_WINDOW_GLTEXTUREPANE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

#include "GLTexture.hpp"
#include "GLShaderProgram.hpp"
#include "IPane.hpp"

namespace basil {

/**
 * @class GLTexturePane
 * @brief Pane which renders to screen at a given position,
 *  using shaders and textures provided.
*/
class GLTexturePane : public IPane, private BasilContextConsumer {
 public:
    /**
     * @brief Creates GLTexturePane from existing GLShaderProgram.
     *
     * @param paneProps     Pane size properties
     * @param shaderProgram GLShaderProgram object to render
    */
    explicit GLTexturePane(
      const PaneProps& paneProps,
      std::shared_ptr<GLShaderProgram> shaderProgram)
        : vertexAttributeID(),
          vertexBufferID(),
          elementBufferID(),
          shaderProgram(shaderProgram),
          IPane(paneProps) {
            setup();
          }

    /** @brief Destructor unbinds OpenGL-allocated memory. */
    ~GLTexturePane();

    /** @param newTexture Texture to pass to shader in each draw call. */
    void addTexture(IGLTexture* newTexture);

    /** @brief Draws to screen using shader and texture(s). */
    void const draw() override;

#ifndef TEST_BUILD

 private:
#endif
    void setup();
    void createVertexObjects();
    void createElementBuffer();

    GLuint vertexAttributeID, vertexBufferID, elementBufferID;
    std::vector<IGLTexture*> textureList;
    std::shared_ptr<GLShaderProgram> shaderProgram;
};

}  // namespace basil

#endif  // SRC_WINDOW_GLTEXTUREPANE_HPP_
