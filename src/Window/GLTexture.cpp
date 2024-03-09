#include "GLTexture.hpp"

namespace basil {

template class GLTexture<1>;
template class GLTexture<2>;
template class GLTexture<3>;

template<>
GLenum GLTexture<1>::getTextureType() {
    return GL_TEXTURE_1D;
}

template<>
GLenum GLTexture<2>::getTextureType() {
    return GL_TEXTURE_2D;
}

template<>
GLenum GLTexture<3>::getTextureType() {
    return GL_TEXTURE_3D;
}

template<int N>
GLTexture<N>::GLTexture() {
    glGenTextures(1, &textureId);
    glActiveTexture(textureEnum);
    glBindTexture(getTextureType(), textureId);

    // TODO(sholloway): Allow for setting & modifying
    glTexParameteri(getTextureType(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(getTextureType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(getTextureType(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(getTextureType(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
}

void IGLTexture::update() {
    glActiveTexture(textureEnum);
    updateGLTexImage();
    glBindTexture(getTextureType(), textureId);
}

template<>
void GLTexture<1>::updateGLTexImage() {
    glTexImage1D(getTextureType(),
                 0,
                 source->format.internalFormat,
                 source->dimension[0],
                 0,
                 source->format.format,
                 source->format.type,
                 source->data());
}

template<>
void GLTexture<2>::updateGLTexImage() {
    glTexImage2D(getTextureType(),
                 0,
                 source->format.internalFormat,
                 source->dimension[0],
                 source->dimension[1],
                 0,
                 source->format.format,
                 source->format.type,
                 source->data());
}

template<>
void GLTexture<3>::updateGLTexImage() {
    glTexImage3D(getTextureType(),
                 0,
                 source->format.internalFormat,
                 source->dimension[0],
                 source->dimension[1],
                 source->dimension[2],
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
