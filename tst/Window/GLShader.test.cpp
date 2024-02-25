#include <sstream>

#include <catch.hpp>

#include <Basil/Window.hpp>

using basil::Logger;
using basil::LogLevel;
using basil::GLShader;
using basil::GLVertexShader;
using basil::GLFragmentShader;

TEST_CASE("Window_GLShader_getShaderFromFile") {
    GLShader shader = GLShader();
    std::filesystem::path testPath = TEST_DIR;

    Logger& logger = Logger::get();
    logger.clearTestInfo();

    SECTION("Reads valid file successfully") {
        std::filesystem::path filePath =
            testPath.append("Window/assets/valid-file.txt");
        shader.getShaderFromFile(filePath);

        REQUIRE(shader.rawShaderCode == "test-message");
        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() ==
            "[INFO]: Shader file read successfully.\n");
    }

    SECTION("Prints error for missing file") {
        std::filesystem::path filePath =
            testPath.append("Window/assets/missing-file.txt");
        shader.getShaderFromFile(filePath);

        REQUIRE(shader.rawShaderCode == "");
        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
        logger.clearTestInfo();
    }
}

TEST_CASE("Window_GLShader_compileShader") {
    Logger& logger = Logger::get();
    GLShader shader = GLShader();

    SECTION("Compiles valid shader code successfully") {
        shader.shaderCode = "#version 330 core\nvoid main() {}\0";
        shader.compileShader(GLShader::ShaderType::FRAGMENT);

        REQUIRE(logger.getLastOutput() ==
            "[INFO]: Shader compiled successfully.\n");
    }

    SECTION("Fails to compile invalid shader code") {
        shader.shaderCode = "fdsjkalfbrejwkl";
        shader.compileShader(GLShader::ShaderType::FRAGMENT);

        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
        logger.clearTestInfo();
    }
}

TEST_CASE("Window_GLVertexShader_GLVertexShader") {
    Logger& logger = Logger::get();
    logger.setLevel(LogLevel::DEBUG);

    std::string successMessage =
        std::string("[INFO]: Shader file read successfully.\n")
        + "[INFO]: Shader compiled successfully.\n";

    std::filesystem::path testPath = TEST_DIR;

    SECTION("Compiles vertex shader successfully") {
        std::filesystem::path filePath =
            testPath.append("Window/assets/test.vert");
        GLVertexShader shader = GLVertexShader(filePath);

        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE(shader.getID() == 1);
    }
}

TEST_CASE("Window_GLVertexShader_noOpShader") {
    Logger& logger = Logger::get();
    logger.setLevel(LogLevel::DEBUG);

    std::string successMessage =
        "[INFO]: Shader compiled successfully.\n";

    std::filesystem::path testPath = TEST_DIR;

    SECTION("Compiles vertex shader successfully") {
        GLVertexShader shader = GLVertexShader::noOpShader();

        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE(shader.getID() == 1);
    }
}

TEST_CASE("Window_GLFragmentShader_GLFragmentShader") {
    Logger& logger = Logger::get();
    logger.setLevel(LogLevel::DEBUG);

    std::filesystem::path testPath = TEST_DIR;

    SECTION("Compiles fragment shader successfully") {
        std::filesystem::path filePath =
            testPath.append("Window/assets/test.frag");
        GLFragmentShader shader = GLFragmentShader(filePath);

        std::string successMessage =
            std::string("[INFO]: Shader file read successfully.\n")
            + "[INFO]: Shader compiled successfully.\n";

        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE(shader.getID() == 1);
    }

    SECTION("Compiles fragment shader from string") {
        std::string shaderCode = "#version 330 core\n"
            "uniform int myUniformInt;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "vec2 st = vec2(gl_FragCoord.x * myUniformInt, "
                "gl_FragCoord.y / 480);\n"
                "gl_FragColor = vec4(st.x, st.y, 0.0, 1.0); }\0";
        std::string successMessage =
            "[INFO]: Shader compiled successfully.\n";

        GLFragmentShader shader = GLFragmentShader(shaderCode);

        CHECK(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE(shader.getID() == 1);
    }
}
