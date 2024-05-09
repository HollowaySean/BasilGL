#ifndef SRC_DATA_TEXTUREFORMAT_HPP_
#define SRC_DATA_TEXTUREFORMAT_HPP_

#include <GL/glew.h>

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
            getFormat<T, channels>(),
            getInternalFormat<T, channels>()
        };
    }

    template<typename T>
    static constexpr GLenum getType();

    template<typename T, int channels>
    static constexpr GLenum getFormat();

    template<typename T, int channels>
    static constexpr GLenum getInternalFormat();
};

template<> constexpr GLenum
    TextureFormat::getType<float>()         { return GL_FLOAT;            }
template<> constexpr GLenum
    TextureFormat::getType<int>()           { return GL_INT;              }
template<> constexpr GLenum
    TextureFormat::getType<u_int>()         { return GL_UNSIGNED_INT;     }
template<> constexpr GLenum
    TextureFormat::getType<char>()          { return GL_BYTE;             }
template<> constexpr GLenum
    TextureFormat::getType<u_char>()        { return GL_UNSIGNED_BYTE;    }

template<> constexpr GLenum
    TextureFormat::getFormat<float, 1>()    { return GL_RED;              }
template<> constexpr GLenum
    TextureFormat::getFormat<int, 1>()      { return GL_RED_INTEGER;      }
template<> constexpr GLenum
    TextureFormat::getFormat<u_int, 1>()    { return GL_RED_INTEGER;      }
template<> constexpr GLenum
    TextureFormat::getFormat<char, 1>()     { return GL_RED;              }
template<> constexpr GLenum
    TextureFormat::getFormat<u_char, 1>()   { return GL_RED;              }

template<> constexpr GLenum
    TextureFormat::getFormat<float, 2>()    { return GL_RG;               }
template<> constexpr GLenum
    TextureFormat::getFormat<int, 2>()      { return GL_RG_INTEGER;       }
template<> constexpr GLenum
    TextureFormat::getFormat<u_int, 2>()    { return GL_RG_INTEGER;       }
template<> constexpr GLenum
    TextureFormat::getFormat<char, 2>()     { return GL_RG;               }
template<> constexpr GLenum
    TextureFormat::getFormat<u_char, 2>()   { return GL_RG;               }

template<> constexpr GLenum
    TextureFormat::getFormat<float, 3>()    { return GL_RGB;              }
template<> constexpr GLenum
    TextureFormat::getFormat<int, 3>()      { return GL_RGB_INTEGER;      }
template<> constexpr GLenum
    TextureFormat::getFormat<u_int, 3>()    { return GL_RGB_INTEGER;      }
template<> constexpr GLenum
    TextureFormat::getFormat<char, 3>()     { return GL_RGB;              }
template<> constexpr GLenum
    TextureFormat::getFormat<u_char, 3>()   { return GL_RGB;              }

template<> constexpr GLenum
    TextureFormat::getFormat<float, 4>()    { return GL_RGBA;             }
template<> constexpr GLenum
    TextureFormat::getFormat<int, 4>()      { return GL_RGBA_INTEGER;     }
template<> constexpr GLenum
    TextureFormat::getFormat<u_int, 4>()    { return GL_RGBA_INTEGER;     }
template<> constexpr GLenum
    TextureFormat::getFormat<char, 4>()     { return GL_RGBA;             }
template<> constexpr GLenum
    TextureFormat::getFormat<u_char, 4>()   { return GL_RGBA;             }

template<> constexpr GLenum
    TextureFormat::getInternalFormat<float, 1>()    { return GL_R32F;     }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<int, 1>()      { return GL_R32I;     }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<u_int, 1>()    { return GL_R32UI;    }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<char, 1>()     { return GL_R8;       }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<u_char, 1>()   { return GL_R8;       }

template<> constexpr GLenum
    TextureFormat::getInternalFormat<float, 2>()    { return GL_RG32F;    }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<int, 2>()      { return GL_RG32I;    }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<u_int, 2>()    { return GL_RG32UI;   }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<char, 2>()     { return GL_RG8;      }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<u_char, 2>()   { return GL_RG8;      }

template<> constexpr GLenum
    TextureFormat::getInternalFormat<float, 3>()    { return GL_RGB32F;   }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<int, 3>()      { return GL_RGB32I;   }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<u_int, 3>()    { return GL_RGB32UI;  }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<char, 3>()     { return GL_RGB8;     }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<u_char, 3>()   { return GL_RGB8;     }

template<> constexpr GLenum
    TextureFormat::getInternalFormat<float, 4>()    { return GL_RGBA32F;  }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<int, 4>()      { return GL_RGBA32I;  }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<u_int, 4>()    { return GL_RGBA32UI; }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<char, 4>()     { return GL_RGBA8;    }
template<> constexpr GLenum
    TextureFormat::getInternalFormat<u_char, 4>()   { return GL_RGBA8;    }


}  // namespace basil

#endif  // SRC_DATA_TEXTUREFORMAT_HPP_
