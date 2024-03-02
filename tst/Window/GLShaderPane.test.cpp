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
using basil::GLShaderPane;
using basil::PaneProps;
using basil::IPane;

template<class T>
using s_p = std::shared_ptr<T>;

TEST_CASE("Window_GLShaderPane_GLShaderPane") {
    SECTION("Creates pane from shader program") { BASIL_LOCK_TEST
        s_p<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        s_p<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);
        s_p<GLShaderProgram> shaderProgram =
            std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

        PaneProps props = {
            .width = 200,
            .height = 100,
            .xOffset = 10,
            .yOffset = 0
        };

        GLShaderPane pane = GLShaderPane(
            props, shaderProgram);

        REQUIRE(pane.vertexAttributeID > 0);
        REQUIRE(pane.vertexBufferID > 0);
        REQUIRE(pane.elementBufferID > 0);
    }

    SECTION("Creates uninitialized pane") {
        GLShaderPane pane = GLShaderPane();

        REQUIRE(pane.vertexAttributeID == 0);
        REQUIRE(pane.vertexBufferID == 0);
        REQUIRE(pane.elementBufferID == 0);
    }
}

TEST_CASE("Window_GLShaderPane_setShaderProgram") {
    SECTION("Does nothing if input is null") {
        GLShaderPane pane = GLShaderPane();
        pane.setShaderProgram(nullptr);

        REQUIRE(pane.shaderProgram == nullptr);
        REQUIRE(pane.vertexAttributeID == 0);
        REQUIRE(pane.vertexBufferID == 0);
        REQUIRE(pane.elementBufferID == 0);
    }

    SECTION("Sets up OpenGL objects if new shader program provided") {
            BASIL_LOCK_TEST
        GLTextureProps textureProps = {
            .name = "textureName",
            .width = 10,
            .height = 20,
            .format = GL_RED,
            .internalFormat = GL_R32F,
            .dataType = GL_FLOAT
        };
        std::vector<float> data = { 1.0 };
        auto texture = std::make_shared<GLTexture<float>>(data, textureProps);

        std::shared_ptr<GLShaderProgram> program =
            GLShaderProgram::Builder()
                .withFragmentShaderFromFile(fragmentPath)
                .withDefaultVertexShader()
                .build();

        GLShaderPane pane = GLShaderPane();
        pane.addTexture(texture);
        pane.setShaderProgram(program);

        REQUIRE(pane.shaderProgram == program);
        REQUIRE_FALSE(pane.vertexAttributeID == 0);
        REQUIRE_FALSE(pane.vertexBufferID == 0);
        REQUIRE_FALSE(pane.elementBufferID == 0);
    }

    SECTION("Replaces previous program if one exists") { BASIL_LOCK_TEST
        std::shared_ptr<GLShaderProgram> program =
            GLShaderProgram::Builder()
                .withFragmentShaderFromFile(fragmentPath)
                .withDefaultVertexShader()
                .build();
        std::shared_ptr<GLShaderProgram> anotherProgram =
            GLShaderProgram::Builder()
                .withFragmentShaderFromFile(fragmentPath)
                .withDefaultVertexShader()
                .build();

        GLShaderPane pane = GLShaderPane();
        pane.setShaderProgram(program);
        REQUIRE(pane.shaderProgram == program);

        REQUIRE_FALSE(pane.vertexAttributeID == 0);
        REQUIRE_FALSE(pane.vertexBufferID == 0);
        REQUIRE_FALSE(pane.elementBufferID == 0);

        int VAO = pane.vertexAttributeID;
        int VBO = pane.vertexBufferID;
        int EBO = pane.elementBufferID;

        pane.setShaderProgram(anotherProgram);
        REQUIRE(pane.shaderProgram == anotherProgram);

        REQUIRE(pane.vertexAttributeID == VAO);
        REQUIRE(pane.vertexBufferID == VBO);
        REQUIRE(pane.elementBufferID == EBO);
    }
}

TEST_CASE("Window_GLShaderPane_addTexture") { BASIL_LOCK_TEST
    GLTextureProps textureProps = {
        .name = "textureName",
        .width = 10,
        .height = 20,
        .format = GL_RED,
        .internalFormat = GL_R32F,
        .dataType = GL_FLOAT
    };
    std::vector<float> data = { 1.0 };
    auto texture = std::make_shared<GLTexture<float>>(data, textureProps);

    GLShaderPane pane = GLShaderPane();

    SECTION("Adds texture to list") {
        pane.addTexture(texture);

        auto actual = pane.textureList.back();
        REQUIRE(actual == texture);
    }
}

TEST_CASE("Window_GLShaderPane_draw") { BASIL_LOCK_TEST
    s_p<GLVertexShader> vertexShader =
        std::make_shared<GLVertexShader>(vertexPath);
    s_p<GLFragmentShader> fragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);
    s_p<GLShaderProgram> shaderProgram =
        std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

    PaneProps paneProps = {
        .width = 20,
        .height = 40,
        .xOffset = 10,
        .yOffset = 0
    };

    GLShaderPane pane = GLShaderPane(
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
    auto texture = std::make_shared<GLTexture<float>>(data, textureProps);

    pane.addTexture(texture);
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

        REQUIRE(texture->getID() == ID);
    }

    SECTION("Binds the vertex array") {
        GLint ID;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &ID);

        REQUIRE(pane.vertexAttributeID == ID);
    }
}

TEST_CASE("Window_GLShaderPane_Builder") { BASIL_LOCK_TEST
    SECTION("Builds from fragment shader") {
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        auto pane = GLShaderPane::Builder()
            .fromShader(fragmentShader)
            .build();

        REQUIRE_FALSE(pane->shaderProgram == nullptr);
        REQUIRE(pane->shaderProgram->fragmentShader == fragmentShader);
    }

    SECTION("Builds from shader file path") {
        auto pane = GLShaderPane::Builder()
            .fromShaderFile(fragmentPath)
            .build();

        REQUIRE_FALSE(pane->shaderProgram == nullptr);
        REQUIRE_FALSE(pane->shaderProgram->fragmentShader == nullptr);
        REQUIRE_FALSE(pane->shaderProgram->vertexShader == nullptr);
    }

    SECTION("Builds from shader code") {
        auto pane = GLShaderPane::Builder()
            .fromShaderCode(validShaderCode)
            .build();

        REQUIRE_FALSE(pane->shaderProgram == nullptr);
        REQUIRE_FALSE(pane->shaderProgram->fragmentShader == nullptr);
        REQUIRE_FALSE(pane->shaderProgram->vertexShader == nullptr);
        REQUIRE(pane->shaderProgram->
            fragmentShader->rawShaderCode == validShaderCode);
    }

    SECTION("Builds with shader program") {
        s_p<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        s_p<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);
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
        auto texture = std::make_shared<GLTexture<float>>(data, textureProps);

        auto pane = GLShaderPane::Builder()
            .withShaderProgram(shaderProgram)
            .withTexture(texture)
            .build();

        REQUIRE(pane->shaderProgram == shaderProgram);
        REQUIRE(pane->textureList.back() == texture);
    }
}
