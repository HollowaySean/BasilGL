#ifndef SRC_WINDOW_GLTEXTUREPANE_HPP_
#define SRC_WINDOW_GLTEXTUREPANE_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "GLShaderProgram.hpp"
#include "IPane.hpp"

class GLTexturePane : public IPane {
 public:
    explicit GLTexturePane(GLShaderProgram shaderProgram)
        : texture(), VAO(), VBO(), EBO(), tex(), shaderProgram(shaderProgram) {}
    void setup();
    void draw();
    void setTexture(std::vector<float> *newTexture) {
        texture = newTexture;
    }
 private:
    std::vector<float> *texture;
    GLuint VAO, VBO, EBO, tex;
    GLShaderProgram shaderProgram;
};

#endif  // SRC_WINDOW_GLTEXTUREPANE_HPP_
