#include <catch.hpp>

#include "OpenGL/GLTexture.hpp"
#include "OpenGL/GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::GLTexture1D;
using basil::GLTexture2D;
using basil::GLTexture3D;
using basil::GLTextureCubemap;
using basil::IGLTexture;
using basil::ITextureSource1D;
using basil::Logger;
using basil::LogLevel;
using basil::SpanTextureSource;

TEST_CASE("OpenGL_GLTexture_GLTexture") { BASIL_LOCK_TEST
    SECTION("Initializes 1D texture") {
        GLTexture1D texture = GLTexture1D();

        CHECK(texture.getID() > 0);
        CHECK(texture.getEnum() >= GL_TEXTURE0);
        CHECK(texture.textureType == GL_TEXTURE_1D);
    }

    SECTION("Initializes 2D texture") {
        GLTexture2D texture = GLTexture2D();

        CHECK(texture.getID() > 0);
        CHECK(texture.getEnum() >= GL_TEXTURE0);
        CHECK(texture.textureType == GL_TEXTURE_2D);
    }

    SECTION("Initializes 3D texture") {
        GLTexture3D texture = GLTexture3D();

        CHECK(texture.getID() > 0);
        CHECK(texture.getEnum() >= GL_TEXTURE0);
        CHECK(texture.textureType == GL_TEXTURE_3D);
    }
}

TEST_CASE("OpenGL_GLTexture_update") { BASIL_LOCK_TEST
    SECTION("Logs error if missing texture source") {
        GLTexture2D texture = GLTexture2D();
        texture.update();

        Logger& logger = Logger::get();
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }

    SECTION("Creates OpenGL 1D texture object") {
        std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 8 };
        std::shared_ptr<SpanTextureSource<int, 1, 1>> source
            = std::make_shared<SpanTextureSource<int, 1, 1>>(data);

        GLTexture1D texture = GLTexture1D();
        source->setWidth(8);
        texture.setSource(source);
        texture.update();

        int result[8];
        glActiveTexture(texture.getEnum());
        glBindTexture(GL_TEXTURE_1D, texture.getID());
        glGetTexImage(GL_TEXTURE_1D, 0, source->format.format,
            source->format.type, result);

        for (int i = 0; i < 8; i++) {
            CHECK(result[i] == data.at(i));
        }
    }

    SECTION("Creates OpenGL 2D texture object") {
        std::vector<char> data = { 1, 2, 3, 4, 5, 6, 7, 8 };
        std::shared_ptr<SpanTextureSource<char, 2, 1>> source
            = std::make_shared<SpanTextureSource<char, 2, 1>>();
        source->setSource(data);

        GLTexture2D texture = GLTexture2D();
        source->setWidth(4);
        source->setHeight(2);
        texture.setSource(source);

        char result[8];
        glActiveTexture(texture.getEnum());
        glBindTexture(GL_TEXTURE_2D, texture.getID());
        glGetTexImage(GL_TEXTURE_2D, 0,
            source->format.format, source->format.type, result);

        for (int i = 0; i < 8; i++) {
            CHECK(result[i] == data.at(i));
        }
    }

    SECTION("Creates OpenGL 3D texture object") {
        std::vector<float> data = { 1, 2, 3, 4, 5, 6, 7, 8 };
        std::shared_ptr<SpanTextureSource<float, 3, 1>> source
            = std::make_shared<SpanTextureSource<float, 3, 1>>(data);
        source->setSource(data);

        GLTexture3D texture = GLTexture3D();
        source->setWidth(2);
        source->setHeight(2);
        source->setDepth(2);
        texture.setSource(source);
        texture.update();

        float result[8];
        glActiveTexture(texture.getEnum());
        glBindTexture(GL_TEXTURE_3D, texture.getID());
        glGetTexImage(GL_TEXTURE_3D, 0, source->format.format,
            source->format.type, result);

        for (int i = 0; i < 8; i++) {
            CHECK(result[i] == data.at(i));
        }
    }
}

TEST_CASE("OpenGL_GLTexture_Builder") { BASIL_LOCK_TEST
    SECTION("Builds from file path") {
        auto path = std::filesystem::path(TEST_DIR)
            / "OpenGL/assets/test-img.jpg";

        auto texture = GLTexture2D::Builder()
            .fromFile(path)
            .build();

        CHECK(texture->source->data() != nullptr);
    }

    SECTION("Builds from span") {
        auto source = std::vector<int>(16, 5);

        auto texture = GLTexture1D::Builder()
            .fromSpan(std::span(source))
            .build();

        CHECK(texture->source->data() == source.data());
    }

    SECTION("Sets texture parameters") {
        auto source = std::vector<int>(16, 5);

        auto texture = GLTexture2D::Builder()
            .fromSpan(std::span(source))
            .withParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE)
            .build();

        GLint result;
        glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &result);
        CHECK(result == GL_CLAMP_TO_EDGE);
    }
}

TEST_CASE("OpenGL_GLTextureCubemap_GLTextureCubemap") { BASIL_LOCK_TEST
    SECTION("Initializes cubemap texture") {
        GLTextureCubemap texture = GLTextureCubemap();

        CHECK(texture.getID() > 0);
        CHECK(texture.getEnum() >= GL_TEXTURE0);
        CHECK(texture.textureType == GL_TEXTURE_CUBE_MAP);
    }
}

TEST_CASE("OpenGL_GLTextureCubemap_update") { BASIL_LOCK_TEST
    SECTION("Logs error if missing texture source") {
        GLTextureCubemap texture = GLTextureCubemap();
        texture.update();

        Logger& logger = Logger::get();
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }

    SECTION("Creates OpenGL Cubemap texture object") {
        std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 8 };
        std::shared_ptr<SpanTextureSource<int, 2, 1>> source
            = std::make_shared<SpanTextureSource<int, 2, 1>>(data);

        GLTextureCubemap texture = GLTextureCubemap();
        source->setWidth(8);
        texture.setSource(source, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        texture.update();

        int result[8];
        glActiveTexture(texture.getEnum());
        glBindTexture(GL_TEXTURE_2D, texture.getID());
        glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, source->format.format,
            source->format.type, result);

        for (int i = 0; i < 8; i++) {
            CHECK(result[i] == data.at(i));
        }
    }
}

TEST_CASE("OpenGL_GLTextureCubemap_Builder") {
    auto path = std::filesystem::path(TEST_DIR)
        / "OpenGL/assets/test-img.jpg";

    SECTION("Builds from file path") {
        auto texture = GLTextureCubemap::Builder()
            .fromFile(path, GL_TEXTURE_CUBE_MAP_POSITIVE_X)
            .build();
        auto source = texture->getSources().at(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X);

        CHECK(source->data() != nullptr);
    }

    SECTION("Sets texture parameters") {
        auto source = std::vector<int>(16, 5);

        auto texture = GLTextureCubemap::Builder()
            .withParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)
            .build();

        GLint result;
        glGetTexParameteriv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, &result);
        CHECK(result == GL_CLAMP_TO_EDGE);
    }

    SECTION("Logs error if load times out") {
        GLTextureCubemap::Builder::FILE_LOAD_TIMEOUT =
            basil::FrameClock::duration::zero();

        auto builder = GLTextureCubemap::Builder()
            .fromFile(path, GL_TEXTURE_CUBE_MAP_POSITIVE_X)
            .build();
        CHECK(Logger::get().getLastLevel() == LogLevel::ERROR);
    }
}
