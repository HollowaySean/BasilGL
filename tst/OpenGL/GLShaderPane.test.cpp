#include <catch.hpp>

#include "OpenGL/GLShaderPane.hpp"

#include "GLTestUtils.hpp"

using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLShaderPane;
using basil::ViewArea;
using basil::IPane;

template<class T>
using s_p = std::shared_ptr<T>;

TEST_CASE("OpenGL_GLShaderPane_GLShaderPane") {
    SECTION("Creates pane from shader program") {
        s_p<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        s_p<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);
        s_p<GLShaderProgram> shaderProgram =
            std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

        ViewArea props = {
            .width = 200,
            .height = 100,
            .xOffset = 10,
            .yOffset = 0
        };

        GLShaderPane pane = GLShaderPane(
            props, shaderProgram);

        CHECK(pane.vertexAttributeID > 0);
        CHECK(pane.vertexBufferID > 0);
        CHECK(pane.elementBufferID > 0);
    }

    SECTION("Creates uninitialized pane") {
        GLShaderPane pane = GLShaderPane();

        CHECK(pane.vertexAttributeID == 0);
        CHECK(pane.vertexBufferID == 0);
        CHECK(pane.elementBufferID == 0);
    }
}

TEST_CASE("OpenGL_GLShaderPane_setShaderProgram") {
    SECTION("Does nothing if input is null") {
        GLShaderPane pane = GLShaderPane();
        pane.setShaderProgram(nullptr);

        CHECK(pane.currentShaderProgram == nullptr);
        CHECK(pane.vertexAttributeID == 0);
        CHECK(pane.vertexBufferID == 0);
        CHECK(pane.elementBufferID == 0);
    }

    SECTION("Sets up OpenGL objects if new shader program provided") {

        std::shared_ptr<GLShaderProgram> program =
            GLShaderProgram::Builder()
                .withFragmentShaderFromFile(fragmentPath)
                .withDefaultVertexShader()
                .build();

        GLShaderPane pane = GLShaderPane();
        pane.setShaderProgram(program);

        CHECK(pane.currentShaderProgram == program);
        CHECK_FALSE(pane.vertexAttributeID == 0);
        CHECK_FALSE(pane.vertexBufferID == 0);
        CHECK_FALSE(pane.elementBufferID == 0);
    }

    SECTION("Replaces previous program if one exists") {
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
        CHECK(pane.currentShaderProgram == program);

        CHECK_FALSE(pane.vertexAttributeID == 0);
        CHECK_FALSE(pane.vertexBufferID == 0);
        CHECK_FALSE(pane.elementBufferID == 0);

        unsigned int VAO = pane.vertexAttributeID;
        unsigned int VBO = pane.vertexBufferID;
        unsigned int EBO = pane.elementBufferID;

        pane.setShaderProgram(anotherProgram);
        CHECK(pane.currentShaderProgram == anotherProgram);

        CHECK(pane.vertexAttributeID == VAO);
        CHECK(pane.vertexBufferID == VBO);
        CHECK(pane.elementBufferID == EBO);
    }

    SECTION("Subscribes/unsubscribes data subscribers") {
        auto pane = GLShaderPane();
        auto program =
            GLShaderProgram::Builder()
                .withFragmentShaderFromFile(fragmentPath)
                .withDefaultVertexShader()
                .build();
        auto anotherProgram = std::make_shared<GLShaderProgram>(*program);

        pane.setShaderProgram(program);
        CHECK(pane.subscriptions.contains(program));

        pane.setShaderProgram(anotherProgram);
        CHECK(pane.subscriptions.contains(anotherProgram));
        CHECK_FALSE(pane.subscriptions.contains(program));
    }
}

TEST_CASE("OpenGL_GLShaderPane_draw") {
    s_p<GLVertexShader> vertexShader =
        std::make_shared<GLVertexShader>(vertexPath);
    s_p<GLFragmentShader> fragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);
    s_p<GLShaderProgram> shaderProgram =
        std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

    ViewArea viewArea = {
        .width = 20,
        .height = 40,
        .xOffset = 10,
        .yOffset = 0
    };

    GLShaderPane pane = GLShaderPane(
        viewArea, shaderProgram);
    pane.draw();

    SECTION("Sets viewport size and position") {
        GLint props[4];
        glGetIntegerv(GL_VIEWPORT, props);
        CHECK(props[0] == viewArea.xOffset);
        CHECK(props[1] == viewArea.yOffset);
        CHECK(props[2] == viewArea.width);
        CHECK(props[3] == viewArea.height);
    }

    SECTION("Sets current shader active") {
        GLint ID;
        glGetIntegerv(GL_CURRENT_PROGRAM, &ID);

        CHECK(pane.currentShaderProgram->getID() == (unsigned int)ID);
    }

    SECTION("Binds the vertex array") {
        GLint ID;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &ID);

        CHECK(pane.vertexAttributeID == (unsigned int)ID);
    }
}

TEST_CASE("OpenGL_GLShaderPane_Builder") {
    SECTION("Builds from fragment shader") {
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        auto pane = GLShaderPane::Builder()
            .fromShader(fragmentShader)
            .build();

        REQUIRE_FALSE(pane->currentShaderProgram == nullptr);
        CHECK(pane->currentShaderProgram->fragmentShader == fragmentShader);
    }

    SECTION("Builds from shader file path") {
        auto pane = GLShaderPane::Builder()
            .fromShaderFile(fragmentPath)
            .build();

        REQUIRE_FALSE(pane->currentShaderProgram == nullptr);
        CHECK_FALSE(pane->currentShaderProgram->fragmentShader == nullptr);
        CHECK_FALSE(pane->currentShaderProgram->vertexShader == nullptr);
    }

    SECTION("Builds from shader code") {
        auto pane = GLShaderPane::Builder()
            .fromShaderCode(validShaderCode)
            .build();

        REQUIRE_FALSE(pane->currentShaderProgram == nullptr);
        REQUIRE_FALSE(pane->currentShaderProgram->fragmentShader == nullptr);
        CHECK_FALSE(pane->currentShaderProgram->vertexShader == nullptr);
        CHECK(pane->currentShaderProgram->
            fragmentShader->rawShaderCode == validShaderCode);
    }

    SECTION("Builds with shader program") {
        s_p<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        s_p<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);
        s_p<GLShaderProgram> shaderProgram =
            std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

        auto pane = GLShaderPane::Builder()
            .withShaderProgram(shaderProgram)
            .build();

        CHECK(pane->currentShaderProgram == shaderProgram);
    }
}
