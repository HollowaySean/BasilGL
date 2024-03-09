#include <catch.hpp>

#include <Basil/Window.hpp>

#include "GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::GLTexture;

/**

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

TEST_CASE("Window_GLTexture_GLTexture") { BASIL_LOCK_TEST
    GLTexture<float> firstTexture = GLTexture<float>(
        floatData, props);
    GLTexture<int> secondTexture = GLTexture<int>(
        intData, props);

    SECTION("Sets texture ID incrementally") {
        REQUIRE(secondTexture.getID() == firstTexture.getID() + 1);
    }
}

TEST_CASE("Window_GLTexture_update") { BASIL_LOCK_TEST
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
}

**/
