#ifndef SRC_WINDOW_GLTEXTURE_HPP_
#define SRC_WINDOW_GLTEXTURE_HPP_

#include <fmt/core.h>
#include <GL/glew.h>

#include <memory>
#include <string>
#include <unordered_map>

#include <Basil/Builder.hpp>
#include <Basil/Context.hpp>
#include <Basil/Logging.hpp>

#include "Data/ITextureSource.hpp"
#include "Data/FileTextureSource.hpp"
#include "Data/SpanTextureSource.hpp"

namespace basil {

/**
 * @brief       Interface for texture template class.
 */
class IGLTexture {
 public:
    /** @brief Assign texture memory in OpenGL. */
    IGLTexture() : textureEnum(nextTexture++) {}

    /** @brief Destructor to tear down OpenGL assigned memory. */
    ~IGLTexture() = default;

    /** @return OpenGL-assigned ID of texture. */
    GLuint getID() const { return textureId; }

    /** @return OpenGL-assigend Enum for texture. */
    GLenum getEnum() const { return textureEnum; }

    /** @return Value to pass into shader program setUniform. */
    GLint getUniformLocation() { return textureEnum - GL_TEXTURE0; }

    /** @brief Flushes data from source to texture. */
    virtual void update() = 0;

    /** @brief Calls glGetTexParameter with set values. */
    void setTextureParameter(GLenum parameterName, GLenum value);

#ifndef TEST_BUILD

 protected:
#endif
    Logger& logger = Logger::get();

    void initializeTexture();
    virtual void updateGLTexImage() = 0;

    inline static GLenum nextTexture = GL_TEXTURE0;

    GLenum textureType;
    GLenum textureEnum;
    GLuint textureId;

    static inline constexpr std::string_view LOGGER_SOURCE_MISSING =
        "Texture (ID{:02}) - Unable to update, data source not found.";
    static inline constexpr std::string_view LOGGER_TEXTURE_CREATED =
        "Texture (ID{:02}) - Created {} texture with offset {}.";

    static inline const std::unordered_map<GLenum, std::string_view>
        NAME_LOOKUP = {
            {GL_TEXTURE_1D, "1D"},
            {GL_TEXTURE_2D, "2D"},
            {GL_TEXTURE_3D, "3D"}
        };
};

/**
 * @brief Implementation class of IGLTexture template.
 *
 * @tparam N Dimensionality of texture. Can be 1D, 2D, or 3D.
 */
template<int N>
class GLTexture : public IGLTexture,
                  public IBuildable<GLTexture<N>>,
                  private IBasilContextConsumer {
    static_assert(N > 0 && N <= 3);

 public:
    /** @brief Assign texture memory in OpenGL. */
    GLTexture();

    /** @brief Destructor to tear down OpenGL assigned memory. */
    ~GLTexture();

    /** @brief Flushes data from source to texture. */
    void update() override;

    /** @brief Sets source of texture. */
    void setSource(std::shared_ptr<ITextureSource<N>> setSource) {
        source = setSource;
        update();
    }

    class Builder : public IBuilder<GLTexture<N>> {
     public:
        /** @brief Builds from existing ITextureSource. */
        Builder& withSource(std::shared_ptr<ITextureSource<N>> source);

        /** @brief Builds from image file. */
        Builder& fromFile(std::filesystem::path filePath);

        /** @brief Builds from std::span collection. */
        template<class T, int channels = 4>
        Builder& fromSpan(std::span<T> span) {
            auto source = std::make_shared<
                SpanTextureSource<T, N, channels>>(span);
            return withSource(source);
        }

        /** @brief Sets texture parameters. */
        Builder& withParameter(GLenum parameterName, GLenum value);
    };

#ifndef TEST_BUILD

 private:
#endif
    void updateGLTexImage() override;

    std::shared_ptr<ITextureSource<N>> source;
};

using GLTexture1D = GLTexture<1>;
using GLTexture2D = GLTexture<2>;
using GLTexture3D = GLTexture<3>;

}  // namespace basil

#endif  // SRC_WINDOW_GLTEXTURE_HPP_
