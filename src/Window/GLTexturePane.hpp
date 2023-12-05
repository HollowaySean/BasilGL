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
    static GLTexturePane fromFile(
      std::filesystem::path filePath,
      GLTexture<float> texture);
    explicit GLTexturePane(
      GLShaderProgram shaderProgram,
      GLTexture<float> texture)
        : vertexAttributeID(), vertexBufferID(), elementBufferID(),
          shaderProgram(shaderProgram), texture(texture) {}
    void setup();
    void draw();

 private:
    GLuint vertexAttributeID, vertexBufferID, elementBufferID;
    GLTexture<float> texture;
    GLShaderProgram shaderProgram;
};

#endif  // SRC_WINDOW_GLTEXTUREPANE_HPP_
