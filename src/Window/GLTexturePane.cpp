#include "GLTexturePane.hpp"

void GLTexturePane::setup() {
    // Set up OpenGL
    createVertexObjects();
    createElementBuffer();

    // Assign texture to shader
    // TODO(sholloway): Move this into texture?? Where is name set
    GLuint location =
        glGetUniformLocation(shaderProgram.getID(), "testTexture");
    glUniform1f(location, 0);
}

void GLTexturePane::createVertexObjects() {
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

void GLTexturePane::createElementBuffer() {
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

void const GLTexturePane::draw() {
    // Clear background color
    // TODO(sholloway): Move to top pane
    // glBlendFunc(GL_ONE, GL_ZERO);
    // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);

    // Set current viewport
    glViewport(
        paneProps.xOffset,
        paneProps.yOffset,
        paneProps.width,
        paneProps.height);

    // Use shader
    shaderProgram.use();

    // Update texture(s)
    // TODO(sholloway): Loop over multiple textures
    texture.update();

    // Render quad of triangles
    glBindVertexArray(vertexAttributeID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


