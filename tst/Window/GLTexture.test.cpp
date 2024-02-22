#include <catch.hpp>

#include "GLTestUtils.hpp"
#include <Basil/Window.hpp>

using basil::GLTexture;
using basil::GLTextureProps;

GLTextureProps props = {
    .name = "test",
    .width = 1,
    .height = 1,
    .format = GL_RED,
    .internalFormat = GL_R32F,
    .dataType = GL_FLOAT
};

std::vector<float> floatData { 1.0f };
std::vector<int> intData { 5 };

TEST_CASE("Window_GLTexture_GLTexture") {
    GLTestUtils::initializeGLContext();

    GLTexture<float> firstTexture = GLTexture<float>(
        floatData, props);
    GLTexture<int> secondTexture = GLTexture<int>(
        intData, props);

    SECTION("Increments active texture enum") {
        REQUIRE(firstTexture.textureEnum == GL_TEXTURE0);
        REQUIRE(secondTexture.textureEnum == GL_TEXTURE1);
    }

    SECTION("Sets texture ID incrementally") {
        REQUIRE(secondTexture.getID() == firstTexture.getID() + 1);
    }

    GLTestUtils::deinitialize();
}

TEST_CASE("Window_GLTexture_update") {
    GLTestUtils::initializeGLContext();

    GLTexture<float> texture = GLTexture<float>(
        floatData, props);
    texture.update();

    SECTION("Writes data to texture") {
        const float expected[1] = { 1.0f };
        float actual[1] = { 0.0f };

        glGetTexImage(GL_TEXTURE_2D, 0,
            props.format, props.dataType, actual);

        REQUIRE(expected[0] == actual[0]);
    }

    GLTestUtils::deinitialize();
}
