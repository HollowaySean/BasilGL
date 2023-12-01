#include "GLTexture.hpp"

template class GLTexture<float>;
template class GLTexture<int>;
template class GLTexture<unsigned int>;

template<class T>
GLenum GLTexture<T>::nextTexture = GL_TEXTURE0;

template<class T>
GLTexture<T>::GLTexture(const std::vector<T> &source,
                        const GLTextureProps &props):
        source(source), props(props) {
    textureEnum = nextTexture++;

    glGenTextures(1, &textureId);
    glActiveTexture(textureEnum);
    glBindTexture(GL_TEXTURE_2D, textureId);

    float blank[props.width * props.height] = {0.0f};

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 props.internalFormat,
                 props.width,
                 props.height,
                 0,
                 props.format,
                 props.dataType,
                 blank);
}

template<class T>
void GLTexture<T>::update() {
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
