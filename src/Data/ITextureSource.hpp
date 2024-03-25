#ifndef SRC_DATA_ITEXTURESOURCE_HPP_
#define SRC_DATA_ITEXTURESOURCE_HPP_

#include <GL/glew.h>

#include <array>

namespace basil {

struct TextureFormat {
    GLenum  type;
    GLenum  format;
    GLenum  internalFormat;

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


template <int N>
class ITextureSource {
    static_assert(N > 0 && N <= 3);

 public:
    void setWidth(int width)   { dimension[0] = width; }
    void setHeight(int height) { dimension[1] = height; }
    void setDepth(int depth)   { dimension[2] = depth; }

    int getWidth()  { return dimension[0]; }
    int getHeight() { return dimension[1]; }
    int getDepth()  { return dimension[2]; }

    virtual const void* data() = 0;

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
