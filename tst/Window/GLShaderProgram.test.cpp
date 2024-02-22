#include <catch.hpp>

#include "GLTestUtils.hpp"
#include <Basil/Window.hpp>

using basil::Logger;
using basil::Level;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;

TEST_CASE("Window_GLShaderProgram_GLShaderProgram") {
    Logger& logger = Logger::get();
    std::filesystem::path testPath = TEST_DIR;
    GLTestUtils::initializeGLContext();

    SECTION("Compiles shader program.") {
        std::filesystem::path filePath =
            std::filesystem::path(TEST_DIR) / "Window/assets/test.vert";
        GLVertexShader vertexShader = GLVertexShader(filePath);
        filePath =
            std::filesystem::path(TEST_DIR) / "Window/assets/test.frag";
        GLFragmentShader fragmentShader = GLFragmentShader(filePath);

        logger.clearTestInfo();
        GLShaderProgram shaderProgram = GLShaderProgram(
            vertexShader, fragmentShader);

        std::string successMessage =
            "[INFO]: Shader program compiled successfully.\n";

        CHECK(logger.getLastLevel() == Level::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);
    }

    SECTION("Logs error for failed compilation.") {
        GLVertexShader vertexShader = GLVertexShader(
            std::filesystem::path(TEST_DIR) / "Window/assets/test.vert");
        GLFragmentShader fragmentShader = GLFragmentShader(
            std::filesystem::path(""));

        logger.clearTestInfo();

        GLShaderProgram shaderProgram = GLShaderProgram(
            vertexShader, fragmentShader);

        REQUIRE(logger.getLastLevel() == Level::ERROR);
    }

    GLTestUtils::deinitialize();
}

TEST_CASE("Window_GLShaderProgram_use") {
    GLTestUtils::initializeGLContext();

    SECTION("Sets current program to shader program ID") {
        std::filesystem::path filePath =
            std::filesystem::path(TEST_DIR) / "Window/assets/test.vert";
        GLVertexShader vertexShader = GLVertexShader(filePath);
        filePath =
            std::filesystem::path(TEST_DIR) / "Window/assets/test.frag";
        GLFragmentShader fragmentShader = GLFragmentShader(filePath);
        GLShaderProgram shaderProgram = GLShaderProgram(
            vertexShader, fragmentShader);

        shaderProgram.use();

        GLint currentID;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentID);

        REQUIRE(currentID == shaderProgram.getID());
    }

    GLTestUtils::deinitialize();
}

TEST_CASE("Window_GLShaderProgram_setUniformInt") {
    GLTestUtils::initializeGLContext();
    SECTION("Sets uniform integer in shader program") {
        std::filesystem::path filePath =
            std::filesystem::path(TEST_DIR) / "Window/assets/test.vert";
        GLVertexShader vertexShader = GLVertexShader(filePath);
        filePath =
            std::filesystem::path(TEST_DIR) / "Window/assets/test.frag";
        GLFragmentShader fragmentShader = GLFragmentShader(filePath);

        GLShaderProgram shaderProgram = GLShaderProgram(
            vertexShader, fragmentShader);

        GLint actualValue = 10;
        std::string name = "myUniformInt";

        shaderProgram.use();
        shaderProgram.setUniformInt(10, name);

        GLint location =
            glGetUniformLocation(shaderProgram.getID(), name.c_str());

        GLint setValue;
        glGetUniformiv(shaderProgram.getID(), location, &setValue);

        REQUIRE(setValue == actualValue);
    }

    GLTestUtils::deinitialize();
}
