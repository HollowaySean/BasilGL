#pragma once

#include <span>

#include <GL/glew.h>

#include <filesystem>
#include <map>
#include <memory>
#include <unordered_map>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/Context.hpp>
#include <Basil/Packages/Logging.hpp>

#include "OpenGL/ITextureSource.hpp"
#include "OpenGL/SpanTextureSource.hpp"


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

    LOGGER_FORMAT LOG_SOURCE_MISSING =
        "Texture (ID{:02}) - Unable to update, data source not found.";
    LOGGER_FORMAT LOG_TEXTURE_CREATED =
        "Texture (ID{:02}) - Created {} texture with offset {}.";
    LOGGER_FORMAT LOG_TEXTURE_DELETED =
        "Texture (ID{:02}) - Texture deleted.";

    static inline const std::unordered_map<GLenum, std::string_view>
        NAME_LOOKUP = {
            {GL_TEXTURE_1D,       "1D"},
            {GL_TEXTURE_2D,       "2D"},
            {GL_TEXTURE_3D,       "3D"},
            {GL_TEXTURE_CUBE_MAP, "Cubemap"}
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

/** @brief Implementation of IGLTexture for cubemaps. */
class GLTextureCubemap : public IGLTexture,
                         public IBuildable<GLTextureCubemap>,
                         private IBasilContextConsumer {
 public:
    /** @brief Initialize OpenGL objects for cubemap. */
    GLTextureCubemap();

    /** @brief Delete OpenGL objects for cubemap. */
    ~GLTextureCubemap();

    /** @brief Flushes data from source to texture. */
    void update() override;

    /** @brief Set 2D ITextureSource for cubemap face.
     *  @param source   Pointer to 2D ITextureSource
     *  @param face     GLenum for cube face,
     *                  i.e. GL_TEXTURE_CUBE_MAP_POSITIVE_X */
    void setSource(
            std::shared_ptr<ITextureSource<2>> setSource,
            GLenum face);

    /** @returns Map from GLenums to ITextureSource pointers. */
    std::map<GLenum, std::shared_ptr<ITextureSource<2>>>
    getSources() { return sources; }

    /** @brief Builder pattern for GLTextureCubemap */
    class Builder : public IBuilder<GLTextureCubemap> {
     public:
        /** @brief Builds cubemap face from existing ITextureSource.
         *  @param source   Pointer to 2D ITextureSource
         *  @param face     GLenum for cube face,
         *                  i.e. GL_TEXTURE_CUBE_MAP_POSITIVE_X */
        Builder& withSource(std::shared_ptr<ITextureSource<2>> source,
            GLenum face);

        /** @brief Builds cubemap face from image file.
         *  @param filePath Path to image file
         *  @param face     GLenum for cube face,
         *                  i.e. GL_TEXTURE_CUBE_MAP_POSITIVE_X */
        Builder& fromFile(std::filesystem::path filePath,
            GLenum face);

        /** @brief Sets texture parameters. */
        Builder& withParameter(GLenum parameterName, GLenum value);
    };

 private:
    void updateGLTexImage() override {}  // GCOVR_EXCL_LINE
    void updateGLTexImage(
        GLenum face,
        std::shared_ptr<ITextureSource<2>> source);

    std::map<GLenum, std::shared_ptr<ITextureSource<2>>> sources
        = std::map<GLenum, std::shared_ptr<ITextureSource<2>>>({
            { GL_TEXTURE_CUBE_MAP_POSITIVE_X, nullptr },
            { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, nullptr },
            { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, nullptr },
            { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, nullptr },
            { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, nullptr },
            { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, nullptr }
        });
};

}   // namespace basil
