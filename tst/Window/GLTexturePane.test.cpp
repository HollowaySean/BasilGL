#include <catch.hpp>

#include <Basil/Window.hpp>

#include "TestUtils.hpp"

using basil::BasilContextLock;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::IGLTexture;
using basil::GLTexture;
using basil::GLTextureProps;
using basil::GLTexturePane;
using basil::PaneProps;
using basil::IPane;

template<class T>
using s_p = std::shared_ptr<T>;

std::filesystem::path vertPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/test.vert";
std::filesystem::path fragPath =
    std::filesystem::path(TEST_DIR) / "Window/assets/test.frag";

TEST_CASE("Window_GLTexturePane_GLTexturePane") { BASIL_LOCK_TEST
    s_p<GLVertexShader> vertexShader =
        std::make_shared<GLVertexShader>(vertPath);
    s_p<GLFragmentShader> fragmentShader =
        std::make_shared<GLFragmentShader>(fragPath);
    s_p<GLShaderProgram> shaderProgram =
        std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

    PaneProps props = {
        .width = 200,
        .height = 100,
        .xOffset = 10,
        .yOffset = 0
    };

    SECTION("Creates pane from shader program") {
        GLTexturePane pane = GLTexturePane(
            props, shaderProgram);

        REQUIRE(pane.vertexAttributeID > 0);
        REQUIRE(pane.vertexBufferID > 0);
        REQUIRE(pane.elementBufferID > 0);
    }
}

TEST_CASE("Window_GLTexturePane_addTexture") { BASIL_LOCK_TEST
    s_p<GLVertexShader> vertexShader =
        std::make_shared<GLVertexShader>(vertPath);
    s_p<GLFragmentShader> fragmentShader =
        std::make_shared<GLFragmentShader>(fragPath);
    s_p<GLShaderProgram> shaderProgram =
        std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

    GLTextureProps textureProps = {
        .name = "textureName",
        .width = 10,
        .height = 20,
        .format = GL_RED,
        .internalFormat = GL_R32F,
        .dataType = GL_FLOAT
    };
    std::vector<float> data = { 1.0 };
    GLTexture<float> texture = GLTexture<float>(data, textureProps);

    PaneProps paneProps = {
        .width = 20,
        .height = 40,
        .xOffset = 10,
        .yOffset = 0
    };

    GLTexturePane pane = GLTexturePane(
        paneProps, shaderProgram);

    SECTION("Adds texture to list") {
        pane.addTexture(&texture);

        IGLTexture* actual = pane.textureList.back();
        REQUIRE(actual == &texture);
    }
}

TEST_CASE("Window_GLTexturePane_draw") { BASIL_LOCK_TEST
    s_p<GLVertexShader> vertexShader =
        std::make_shared<GLVertexShader>(vertPath);
    s_p<GLFragmentShader> fragmentShader =
        std::make_shared<GLFragmentShader>(fragPath);
    s_p<GLShaderProgram> shaderProgram =
        std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

    PaneProps paneProps = {
        .width = 20,
        .height = 40,
        .xOffset = 10,
        .yOffset = 0
    };

    GLTexturePane pane = GLTexturePane(
        paneProps, shaderProgram);

    GLTextureProps textureProps = {
        .name = "textureName",
        .width = 10,
        .height = 20,
        .format = GL_RED,
        .internalFormat = GL_R32F,
        .dataType = GL_FLOAT
    };
    std::vector<float> data = { 1.0 };
    GLTexture<float> texture = GLTexture<float>(data, textureProps);

    pane.addTexture(&texture);
    pane.draw();

    SECTION("Sets viewport size and position") {
        GLint props[4];
        glGetIntegerv(GL_VIEWPORT, props);
        REQUIRE(props[0] == paneProps.xOffset);
        REQUIRE(props[1] == paneProps.yOffset);
        REQUIRE(props[2] == paneProps.width);
        REQUIRE(props[3] == paneProps.height);
    }

    SECTION("Sets current shader active") {
        GLint ID;
        glGetIntegerv(GL_CURRENT_PROGRAM, &ID);

        REQUIRE(pane.shaderProgram->getID() == ID);
    }

    SECTION("Binds textures in textureList") {
        GLint ID;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &ID);

        REQUIRE(texture.getID() == ID);
    }

    SECTION("Binds the vertex array") {
        GLint ID;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &ID);

        REQUIRE(pane.vertexAttributeID == ID);
    }
}
