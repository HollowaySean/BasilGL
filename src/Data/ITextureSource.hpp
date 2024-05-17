#pragma once

#include <GL/glew.h>

#include <array>

#include "GLTextureFormat.hpp"

namespace basil {

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
    GLTextureFormat format;

#ifndef TEST_BUILD

 protected:
#endif
    std::array<int, N> dimension = { 0 };
};

using ITextureSource1D = ITextureSource<1>;
using ITextureSource2D = ITextureSource<2>;
using ITextureSource3D = ITextureSource<3>;

}   // namespace basil
