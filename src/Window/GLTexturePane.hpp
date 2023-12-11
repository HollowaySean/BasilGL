#ifndef SRC_WINDOW_GLTEXTUREPANE_HPP_
#define SRC_WINDOW_GLTEXTUREPANE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "GLTexture.hpp"
#include "GLShaderProgram.hpp"
#include "IPane.hpp"

class GLTexturePane : public IPane {
 public:
    explicit GLTexturePane(
      PaneProps paneProps,
      std::filesystem::path filePath,
      const IGLTexture &texture)
        : vertexAttributeID(),
          vertexBufferID(),
          elementBufferID(),
          shaderProgram(GLShaderProgram(
              GLVertexShader::noOpShader(),
              GLFragmentShader(filePath))),
          texture(texture),
          IPane(paneProps) {
            setup();
          }

    explicit GLTexturePane(
      PaneProps paneProps,
      const GLShaderProgram &shaderProgram,
      const IGLTexture &texture)
        : vertexAttributeID(),
          vertexBufferID(),
          elementBufferID(),
          shaderProgram(shaderProgram),
          texture(texture),
          IPane(paneProps) {}

    void const draw() override;

 private:
    void setup();
    void createVertexObjects();
    void createElementBuffer();

    GLuint vertexAttributeID, vertexBufferID, elementBufferID;
    const IGLTexture &texture;
    GLShaderProgram shaderProgram;
};

#endif  // SRC_WINDOW_GLTEXTUREPANE_HPP_
