#include "GLTexturePane.hpp"

void GLTexturePane::setup() {
    // Create Vertex Attribute Object
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Copy vertices of full sized quad into buffer
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

    // Copy indices into element buffer
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    glGenBuffers(1, &elementBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices), indices, GL_STATIC_DRAW);

    // TEST: ASSIGN TO SHADER
    shaderProgram.use();
    GLuint location =
        glGetUniformLocation(shaderProgram.getID(), "testTexture");
    glUniform1f(location, 0);
}

void GLTexturePane::draw() {
    // Clear background color
    glBlendFunc(GL_ONE, GL_ZERO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use shader
    shaderProgram.use();

    // Update texture(s)
    texture.update();

    // Render quad of triangles
    glBindVertexArray(vertexArrayID);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


