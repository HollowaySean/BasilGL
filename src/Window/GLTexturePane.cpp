#include "GLTexturePane.hpp"

void GLTexturePane::setup() {
    // Create Vertex Attribute Object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Copy vertices of full sized quad into buffer
    float vertices[] = {
         // Position              // UV coordinates
         1.0f,  1.0f,  0.0f,      1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,      1.0f,  1.0f,
        -1.0f, -1.0f,  0.0f,      0.0f,  1.0f,
        -1.0f,  1.0f,  0.0f,      0.0f,  0.0f
    };
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set up vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // Copy indices into element buffer
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    float blank[100] = {0.0f};


    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 10, 10, 0, GL_RED, GL_FLOAT, blank);
}

void GLTexturePane::draw() {
    // Clear background color
    glBlendFunc(GL_ONE, GL_ZERO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use shader
    shaderProgram.use();

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, 10, 10, 0, GL_RED, GL_FLOAT, texture);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Render quad of triangles
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


