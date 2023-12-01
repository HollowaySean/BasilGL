#ifndef SRC_WINDOW_GLTEXTURE_HPP_
#define SRC_WINDOW_GLTEXTURE_HPP_

#include <GL/glew.h>

#include <vector>

struct GLTextureProps {
 public:
    GLsizei width;
    GLsizei height;
    GLenum format;
    GLenum internalFormat;
    GLenum dataType;
};

template<class T>
class GLTexture {
 public:
    explicit GLTexture(const std::vector<T> &source,
                       const GLTextureProps &props);
    void update();
 private:
    static GLenum nextTexture;

    GLenum textureEnum;
    const std::vector<T> &source;
    const GLTextureProps &props;
    GLuint textureId;
};

#endif  // SRC_WINDOW_GLTEXTURE_HPP_
