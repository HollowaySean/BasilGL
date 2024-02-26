#include "GLTexture.hpp"

namespace basil {

template class GLTexture<float>;
template class GLTexture<int>;
template class GLTexture<unsigned int>;

template<class T>
GLTexture<T>::GLTexture(std::span<T> source,
                        const GLTextureProps &props):
        source(source), textureEnum(nextTexture++), IGLTexture(props) {
    glGenTextures(1, &textureId);
    glActiveTexture(textureEnum);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // TODO(sholloway): Allow for setting & modifying
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

template<class T>
void GLTexture<T>::update() const {
    glActiveTexture(textureEnum);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 props.internalFormat,
                 props.width,
                 props.height,
                 0,
                 props.format,
                 props.dataType,
                 source.data());
    glBindTexture(GL_TEXTURE_2D, textureId);
}

IGLTexture::~IGLTexture() {
    GLuint textureArray[] = { textureId };
    glDeleteTextures(1, textureArray);
}

}  // namespace basil
