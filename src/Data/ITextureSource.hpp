#ifndef SRC_DATA_ITEXTURESOURCE_HPP_
#define SRC_DATA_ITEXTURESOURCE_HPP_

#include <GL/glew.h>

#include <array>

namespace basil {

/**
 * @brief Struct containing OpenGL enums for defining textures.
 */
struct TextureFormat {
    /** @brief Numeric type of data, i.e. GL_FLOAT, GL_UNSIGNED_BYTE */
    GLenum  type;

    /** @brief Format of texture data, i.e. GL_RED, GL_RGBA */
    GLenum  format;

    /** @brief Format to save texture as, i.e. GL_RED, GL_RGBA */
    GLenum  internalFormat;

    /** @return Builds TextureFormat struct for a given type and number of channels
     *  @tparam T Numeric type of data
     *  @tparam channels Number of channels in image data
    */
    template<typename T, int channels>
    static constexpr TextureFormat getTextureFormat() {
        return {
            getType<T>(),
            getFormat<channels>(),
            getFormat<channels>()
        };
    }

    template<typename T>
    static constexpr GLenum getType();

    template<int channels>
    static constexpr GLenum getFormat();
};

template<> constexpr GLenum
    TextureFormat::getType<float>()  { return GL_FLOAT;         }
template<> constexpr GLenum
    TextureFormat::getType<int>()    { return GL_INT;           }
template<> constexpr GLenum
    TextureFormat::getType<u_int>()  { return GL_UNSIGNED_INT;  }
template<> constexpr GLenum
    TextureFormat::getType<char>()   { return GL_BYTE;          }
template<> constexpr GLenum
    TextureFormat::getType<u_char>() { return GL_UNSIGNED_BYTE; }

template<> constexpr GLenum
    TextureFormat::getFormat<1>()    { return GL_RED;   }
template<> constexpr GLenum
    TextureFormat::getFormat<2>()    { return GL_RG;    }
template<> constexpr GLenum
    TextureFormat::getFormat<3>()    { return GL_RGB;   }
template<> constexpr GLenum
    TextureFormat::getFormat<4>()    { return GL_RGBA;  }

/**
 * @brief Interface for container of data which can be used in GLTexture
 *
 * @tparam N Dimensionality of texture, i.e. 1D, 2D, or 3D.
 */
template <int N>
class ITextureSource {
    static_assert(N > 0 && N <= 3);

 public:
    /** @brief Set width of texture, in pixels. */
    void setWidth(int width)   { dimension[0] = width; }

    /** @brief Set height of 2D or 3D texture, in pixels. */
    void setHeight(int height) { dimension[1] = height; }

    /** @brief Set depth of 3D texture, in pixels. */
    void setDepth(int depth)   { dimension[2] = depth; }

    /** @returns Width, in pixels. */
    int getWidth()  { return dimension[0]; }

    /** @returns Height, in pixels. */
    int getHeight() { return dimension[1]; }

    /** @returns Depth, in pixels.*/
    int getDepth()  { return dimension[2]; }

    /** @brief Provides pointer to data in memory. */
    virtual const void* data() = 0;

    /** @brief Struct containing OpenGL data type information. */
    TextureFormat format;

#ifndef TEST_BUILD

 protected:
#endif
    std::array<int, N> dimension = { 0 };
};

using ITextureSource1D = ITextureSource<1>;
using ITextureSource2D = ITextureSource<2>;
using ITextureSource3D = ITextureSource<3>;

}  // namespace basil

#endif  // SRC_DATA_ITEXTURESOURCE_HPP_
