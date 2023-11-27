#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <sstream>

#include <catch.hpp>

#include "GLShader.hpp"

void initializeGLContext() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, false);
    GLFWwindow* newWindow = glfwCreateWindow(
        1, 1, "test",
        NULL, NULL);
    glfwMakeContextCurrent(newWindow);
    glewInit();
}

TEST_CASE("Window_GLShader_getShaderFromFile") {
    Logger& logger = Logger::get();
    GLShader shader = GLShader();
    std::filesystem::path testPath = TEST_DIR;

    SECTION("Reads valid file successfully") {
        std::filesystem::path filePath =
            testPath.append("Window/assets/valid-file.txt");
        shader.getShaderFromFile(filePath);

        REQUIRE(shader.rawShaderCode == "test-message");
        REQUIRE(logger.getLastLevel() == Level::INFO);
        REQUIRE(logger.getLastOutput() ==
            "[INFO]: Shader file read successfully.\n");
    }

    SECTION("Prints error for missing file") {
        std::filesystem::path filePath =
            testPath.append("Window/assets/missing-file.txt");
        shader.getShaderFromFile(filePath);

        REQUIRE(shader.rawShaderCode == "");
        REQUIRE(logger.getLastLevel() == Level::ERROR);
        logger.clearTestInfo();
    }
}

TEST_CASE("Window_GLShader_compileShader") {
    Logger& logger = Logger::get();
    GLShader shader = GLShader();
    initializeGLContext();

    SECTION("Compiles valid shader code successfully") {
        shader.shaderCode = "#version 330 core\nvoid main() {}\0";
        shader.compileShader(GLShader::ShaderType::FRAGMENT);

        REQUIRE(logger.getLastOutput() ==
            "[INFO]: Shader compiled successfully.\n");
    }

    SECTION("Fails to compile invalid shader code") {
        shader.shaderCode = "fdsjkalfbrejwkl";
        shader.compileShader(GLShader::ShaderType::FRAGMENT);

        REQUIRE(logger.getLastLevel() == Level::ERROR);
        logger.clearTestInfo();
    }
}

TEST_CASE("Window_GLVertexShader_GLVertexShader") {
    Logger& logger = Logger::get();
    logger.setLevel(Level::DEBUG);

    std::string successMessage =
        std::string("[INFO]: Shader file read successfully.\n")
        + "[INFO]: Shader compiled successfully.\n";

    std::filesystem::path testPath = TEST_DIR;
    initializeGLContext();

    SECTION("Compiles vertex shader successfully") {
        std::filesystem::path filePath =
            testPath.append("Window/assets/test.vert");
        GLVertexShader shader = GLVertexShader(filePath);

        REQUIRE(logger.getLastLevel() == Level::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE(shader.getID() == 1);
    }
}


TEST_CASE("Window_GLFragmentShader_GLFragmentShader") {
    Logger& logger = Logger::get();
    logger.setLevel(Level::DEBUG);

    std::string successMessage =
        std::string("[INFO]: Shader file read successfully.\n")
        + "[INFO]: Shader compiled successfully.\n";

    std::filesystem::path testPath = TEST_DIR;
    initializeGLContext();

    SECTION("Compiles fragment shader successfully") {
        std::filesystem::path filePath =
            testPath.append("Window/assets/test.frag");
        GLFragmentShader shader = GLFragmentShader(filePath);

        REQUIRE(logger.getLastLevel() == Level::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE(shader.getID() == 1);
    }
}
