#include <catch.hpp>

#include <Basil/Data.hpp>
#include <Basil/Window.hpp>

#include "GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::GLTexture1D;
using basil::GLTexture2D;
using basil::GLTexture3D;
using basil::IGLTexture;
using basil::ITextureSource1D;
using basil::Logger;
using basil::LogLevel;
using basil::SpanTextureSource;

TEST_CASE("Window_GLTexture_GLTexture") { BASIL_LOCK_TEST
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

TEST_CASE("Window_GLTexture_update") { BASIL_LOCK_TEST
    std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 8 };

    SECTION("Logs error if missing texture source") {
        GLTexture2D texture = GLTexture2D();
        texture.update();

        Logger& logger = Logger::get();
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }

    SECTION("Creates OpenGL 1D texture object") {
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

        // TODO(sholloway): Complete tests after fixing
    }

    SECTION("Creates OpenGL 2D texture object") {
        std::shared_ptr<SpanTextureSource<int, 2, 1>> source
            = std::make_shared<SpanTextureSource<int, 2, 1>>();
        source->setSource(data);

        GLTexture2D texture = GLTexture2D();
        source->setWidth(4);
        source->setHeight(2);
        texture.setSource(source);
        texture.update();

        int result[8];
        glActiveTexture(texture.getEnum());
        glBindTexture(GL_TEXTURE_2D, texture.getID());
        glGetTexImage(GL_TEXTURE_2D, 0, source->format.format,
            source->format.type, result);

        // TODO(sholloway): Complete tests after fixing
    }

    SECTION("Creates OpenGL 3D texture object") {
        std::shared_ptr<SpanTextureSource<int, 3, 1>> source
            = std::make_shared<SpanTextureSource<int, 3, 1>>(data);
        source->setSource(data);

        GLTexture3D texture = GLTexture3D();
        source->setWidth(2);
        source->setHeight(2);
        source->setDepth(2);
        texture.setSource(source);
        texture.update();

        int result[8];
        glActiveTexture(texture.getEnum());
        glBindTexture(GL_TEXTURE_3D, texture.getID());
        glGetTexImage(GL_TEXTURE_3D, 0, source->format.format,
            source->format.type, result);

        // TODO(sholloway): Complete tests after fixing
    }
}

TEST_CASE("Window_GLTexture_Builder") {
    SECTION("Builds from file path") {
        auto path = std::filesystem::path(TEST_DIR)
            / "Data/assets/test-img.jpg";

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
}
