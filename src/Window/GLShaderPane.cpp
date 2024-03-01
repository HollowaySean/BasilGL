#include "GLShaderPane.hpp"

namespace basil {

void GLShaderPane::setup() {
    // Set up OpenGL
    createVertexObjects();
    createElementBuffer();
}

void GLShaderPane::addTexture(std::shared_ptr<IGLTexture> newTexture) {
    // Add texture to list
    textureList.push_back(newTexture);

    // Assign texture to shader
    GLuint location =
        glGetUniformLocation(shaderProgram->getID(), newTexture->props.name);
    glUniform1f(location, 0);
}

void GLShaderPane::createVertexObjects() {
    // Create Vertex Attribute Object
    glGenVertexArrays(1, &vertexAttributeID);
    glBindVertexArray(vertexAttributeID);

    // Copy vertices of unit quad into buffer
    float vertices[] = {
         // Position              // UV coordinates
         1.0f,  1.0f,  0.0f,      1.0f,  1.0f,
         1.0f, -1.0f,  0.0f,      1.0f,  0.0f,
        -1.0f, -1.0f,  0.0f,      0.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,      0.0f,  1.0f
    };
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void GLShaderPane::createElementBuffer() {
    // Copy indices into element buffer
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    glGenBuffers(1, &elementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices), indices, GL_STATIC_DRAW);
}

void const GLShaderPane::draw() {
    // Set current viewport
    glViewport(
        paneProps.xOffset,
        paneProps.yOffset,
        paneProps.width,
        paneProps.height);

    // Use shader
    shaderProgram->use();

    // Update texture(s)
    for (std::shared_ptr<IGLTexture> texture : textureList) {
        texture->update();
    }

    // Render quad of triangles
    glBindVertexArray(vertexAttributeID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLShaderPane::~GLShaderPane() {
    GLuint vertexArrays[] = { vertexAttributeID };
    glDeleteVertexArrays(1, vertexArrays);

    GLuint bufferArrays[] = { elementBufferID, vertexBufferID };
    glDeleteBuffers(2, bufferArrays);
}

}  // namespace basil
