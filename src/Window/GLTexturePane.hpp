#ifndef SRC_WINDOW_GLTEXTUREPANE_HPP_
#define SRC_WINDOW_GLTEXTUREPANE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "GLTexture.hpp"
#include "GLShaderProgram.hpp"
#include "IPane.hpp"

// TODO(sholloway):
//  Documentation
//  Tests
//  Figure out best way to store or create shaderProgram
class GLTexturePane : public IPane {
 public:
    explicit GLTexturePane(
      PaneProps paneProps,
      std::filesystem::path filePath)
        : vertexAttributeID(),
          vertexBufferID(),
          elementBufferID(),
          shaderProgram(GLShaderProgram(
              GLVertexShader::noOpShader(),
              GLFragmentShader(filePath))),
          IPane(paneProps) {
            setup();
          }

    explicit GLTexturePane(
      PaneProps paneProps,
      const GLShaderProgram &shaderProgram)
        : vertexAttributeID(),
          vertexBufferID(),
          elementBufferID(),
          shaderProgram(shaderProgram),
          IPane(paneProps) {
            setup();
          }

    ~GLTexturePane();

    void addTexture(IGLTexture* newTexture);

    void const draw() override;

 private:
    void setup();
    void createVertexObjects();
    void createElementBuffer();

    GLuint vertexAttributeID, vertexBufferID, elementBufferID;
    std::vector<IGLTexture*> textureList;
    GLShaderProgram shaderProgram;
};

#endif  // SRC_WINDOW_GLTEXTUREPANE_HPP_
