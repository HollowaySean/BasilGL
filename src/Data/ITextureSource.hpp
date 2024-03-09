#ifndef SRC_DATA_ITEXTURESOURCE_HPP_
#define SRC_DATA_ITEXTURESOURCE_HPP_

#include <GL/glew.h>

namespace basil {

struct TextureFormat {
    GLenum  format;
    GLenum  type;
    GLenum  internalFormat;
};

template <int N>
class ITextureSource {
    static_assert(N > 0 && N <= 3);

 public:
    virtual const void* data() = 0;

// protected:
    TextureFormat format;
    int dimension[N] = { 0 };
};

using ITextureSource1D = ITextureSource<1>;
using ITextureSource2D = ITextureSource<2>;
using ITextureSource3D = ITextureSource<3>;

}  // namespace basil

#endif  // SRC_DATA_ITEXTURESOURCE_HPP_
