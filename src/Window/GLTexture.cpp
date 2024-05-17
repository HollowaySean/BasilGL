#include <fmt/format.h>

#include "Data/FileTextureSource.hpp"

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

void IGLTexture::setTextureParameter(GLenum parameterName, GLenum value) {
    glTexParameteri(textureType, parameterName, value);
}

void IGLTexture::initializeTexture() {
    glGenTextures(1, &textureId);
    glActiveTexture(textureEnum);
    glBindTexture(textureType, textureId);

    logger.log(
        fmt::format(LOG_TEXTURE_CREATED,
            textureId,
            NAME_LOOKUP.at(textureType),
            static_cast<int>(textureEnum - GL_TEXTURE0)),
        LogLevel::DEBUG);

    // Set default texture parameters
    setTextureParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    setTextureParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    setTextureParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    setTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

template<int N>
void GLTexture<N>::update() {
    if (!source) {
        logger.log(
            fmt::format(LOG_SOURCE_MISSING, textureId),
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

template<int N>
GLTexture<N>::Builder&
GLTexture<N>::Builder::withSource(std::shared_ptr<ITextureSource<N>> source) {
    this->impl->setSource(source);
    return (*this);
}

template<>
GLTexture<2>::Builder&
GLTexture<2>::Builder::fromFile(std::filesystem::path filePath) {
    auto source = std::make_shared<FileTextureSource>(filePath);
    return withSource(source);
}

template<int N>
GLTexture<N>::Builder&
GLTexture<N>::Builder::withParameter(GLenum parameterName, GLenum value) {
    this->impl->setTextureParameter(parameterName, value);
    return (*this);
}

}  // namespace basil
