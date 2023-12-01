#ifndef SRC_WINDOW_GLTEXTURE_HPP_
#define SRC_WINDOW_GLTEXTURE_HPP_

#include <GL/glew.h>

#include <vector>

/** @brief Struct used to pass properties of texture to GLTexture. */
struct GLTextureProps {
 public:
    /** @brief Width of texture. */
    GLsizei width;
    /** @brief Height of texture. */
    GLsizei height;
    /** @brief Format of texture data, i.e. GL_RED */
    GLenum format;
    /** @brief Numerical format of data, i.e. GL_R32F */
    GLenum internalFormat;
    /** @brief Numerical type of input data, i.e. GL_FLOAT */
    GLenum dataType;
};

/**
 * @brief       Internal class to contain non-template static variable.
 * @attention   Do not use externally.
 */
class GLTextureGeneric {
 protected:
    GLTextureGeneric() = default;
    static GLenum nextTexture;
};

/**
 * @brief       Wrapper class for managing OpenGL textures.
 *
 * @tparam T    Data type of texture data source.
 */
template<class T>
class GLTexture : private GLTextureGeneric {
 public:
    /**
     * @brief           Construct a new GLTexture object and
     *                  initialize OpenGL texture.
     *
     * @param source    Vector containing texture data.
     * @param props     GLTextureProps structure.
     */
    explicit GLTexture(const std::vector<T> &source,
                       const GLTextureProps &props);

    /** @brief Flush data from source to texture.*/
    void update();

#ifndef TEST_BUILD
 private:
#endif
    GLenum textureEnum;
    const std::vector<T> &source;
    const GLTextureProps &props;
    GLuint textureId;
};

#endif  // SRC_WINDOW_GLTEXTURE_HPP_
