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
    explicit GLTexturePane(GLShaderProgram shaderProgram,
                           GLTexture<float> texture)
        : vertexArrayID(), vertexBufferID(), elementBufferID(),
          shaderProgram(shaderProgram), texture(texture) {}
    void setup();
    void draw();

 private:
    GLuint vertexArrayID, vertexBufferID, elementBufferID;
    GLTexture<float> texture;
    GLShaderProgram shaderProgram;
};

#endif  // SRC_WINDOW_GLTEXTUREPANE_HPP_
