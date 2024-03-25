#include "GLTexture.hpp"

namespace basil {

template class GLTexture<1>;
template class GLTexture<2>;
template class GLTexture<3>;

template<>
GLTexture<1>::GLTexture() {
    textureType = GL_TEXTURE_1D;
    initializeTexture();
}

template<>
GLTexture<2>::GLTexture() {
    textureType = GL_TEXTURE_2D;
    initializeTexture();
}

template<>
GLTexture<3>::GLTexture() {
    textureType = GL_TEXTURE_3D;
    initializeTexture();
}

void IGLTexture::initializeTexture() {
    glGenTextures(1, &textureId);
    glActiveTexture(textureEnum);
    glBindTexture(textureType, textureId);

    // TODO(sholloway): Figure out better way to handle this
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum error = glGetError();
}

template<int N>
void GLTexture<N>::update() {
    if (!source) {
        logger.log("Unable to update, texture source not found.",
            LogLevel::WARN);
        return;
    }

    glActiveTexture(textureEnum);
    updateGLTexImage();
    glBindTexture(textureType, textureId);
}

template<>
void GLTexture<1>::updateGLTexImage() {
    glTexImage1D(textureType,
                 0,
                 source->format.internalFormat,
                 source->getWidth(),
                 0,
                 source->format.format,
                 source->format.type,
                 source->data());
}

template<>
void GLTexture<2>::updateGLTexImage() {
    glTexImage2D(textureType,
                 0,
                 source->format.internalFormat,
                 source->getWidth(),
                 source->getHeight(),
                 0,
                 source->format.format,
                 source->format.type,
                 source->data());
}

template<>
void GLTexture<3>::updateGLTexImage() {
    glTexImage3D(textureType,
                 0,
                 source->format.internalFormat,
                 source->getWidth(),
                 source->getHeight(),
                 source->getDepth(),
                 0,
                 source->format.format,
                 source->format.type,
                 source->data());
}

template<int N>
GLTexture<N>::~GLTexture() {
    GLuint textureArray[] = { textureId };
    glDeleteTextures(1, textureArray);
}

}  // namespace basil
