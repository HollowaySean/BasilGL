#include "GLShaderProgram.hpp"
#include "GLTestUtils.hpp"

#include <catch.hpp>

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
        GLVertexShader vertexShader = GLVertexShader("");
        GLFragmentShader fragmentShader = GLFragmentShader("");

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
