#ifndef SRC_WINDOW_GLTEXTURE_HPP_
#define SRC_WINDOW_GLTEXTURE_HPP_

#include <GL/glew.h>

#include <string>
#include <vector>

/** @brief Struct used to pass properties of texture to GLTexture. */
struct GLTextureProps {
 public:
    /** @brief Variable name used in shader code. */
    const char* name;
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
 * @brief       Generic interface for template class.
 */
class IGLTexture {
 public:
    /** @brief Flushes data from source to texture. */
    virtual void update() const {}
    /** @return OpenGL-assigned ID of texture*/
    GLuint getID() { return textureId; }

    /** @brief Struct containing texture properties. */
    const GLTextureProps &props;

    /** @brief Destructor to tear down OpenGL assigned memory. */
    virtual ~IGLTexture();

 protected:
    explicit IGLTexture(const GLTextureProps &props): props(props) {}

    static GLenum nextTexture;

    GLuint textureId;
};

/**
 * @brief       Wrapper class for managing OpenGL textures.
 * @tparam T    Data type of texture data source.
 */
template<class T>
class GLTexture : public IGLTexture {
 public:
    /**
     * @brief           Construct a new GLTexture object and
     *                  initialize OpenGL texture.
     * @param source    Vector containing texture data.
     * @param props     GLTextureProps structure.
     */
    explicit GLTexture(const std::vector<T> &source,
                       const GLTextureProps &props);

    /** @brief Flush data from source to texture.*/
    void update() const override;

#ifndef TEST_BUILD
 private:
#endif
    GLenum textureEnum;
    const std::vector<T> &source;
};

#endif  // SRC_WINDOW_GLTEXTURE_HPP_
