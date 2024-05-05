#include <sstream>

#include <catch.hpp>

#include <Basil/Window.hpp>

#include "GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::Logger;
using basil::LogLevel;
using basil::GLShader;
using basil::GLVertexShader;
using basil::GLFragmentShader;

class TestShader : public GLShader {
 public:
    TestShader() = default;
    void setShader(std::filesystem::path path) override {}
    void setShader(const std::string& shaderCode) override {}
};

TEST_CASE("Window_GLShader_getShaderFromFile") { BASIL_LOCK_TEST
    TestShader shader = TestShader();
    Logger& logger = Logger::get();

    SECTION("Reads valid file successfully") {
        shader.getShaderFromFile(validPath);

        REQUIRE(shader.rawShaderCode == "test-message");
        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() ==
            "Shader file read successfully.");
        REQUIRE(shader.hasCompiledSuccessfully());
    }

    SECTION("Prints error for missing file") {
        shader.getShaderFromFile(invalidPath);

        REQUIRE(shader.rawShaderCode == "");
        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
        REQUIRE_FALSE(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("Window_GLShader_compileShader") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();
    TestShader shader = TestShader();

    SECTION("Compiles valid shader code successfully") {
        shader.rawShaderCode = validShaderCode;
        shader.hasCompiled = true;
        shader.compileShader(GLShader::ShaderType::FRAGMENT);

        REQUIRE(logger.getLastOutput() ==
            "Shader compiled successfully.");
        REQUIRE(shader.hasCompiledSuccessfully());
    }

    SECTION("Fails to compile invalid shader code") {
        shader.rawShaderCode = "fdsjkalfbrejwkl";
        shader.hasCompiled = true;
        shader.compileShader(GLShader::ShaderType::FRAGMENT);

        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
        REQUIRE_FALSE(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("Window_GLVertexShader_GLVertexShader") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();
    logger.setLevel(LogLevel::DEBUG);

    std::string successMessage =
        "Shader compiled successfully.";

    SECTION("Compiles vertex shader successfully") {
        GLVertexShader shader = GLVertexShader(vertexPath);

        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE_FALSE(shader.getID() == 0);
        REQUIRE(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("Window_GLVertexShader_noOpShader") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();
    logger.setLevel(LogLevel::DEBUG);

    std::string successMessage =
        "Shader compiled successfully.";

    SECTION("Compiles vertex shader successfully") {
        auto shader = GLVertexShader::noOpShader();

        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE_FALSE(shader->getID() == 0);
        REQUIRE(shader->hasCompiledSuccessfully());
    }
}

TEST_CASE("Window_GLVertexShader_setShader") { BASIL_LOCK_TEST
    GLVertexShader shader = GLVertexShader();

    SECTION("Compiles vertex shader from path") {
        REQUIRE(shader.getID() == 0);

        shader.setShader(vertexPath);
        REQUIRE_FALSE(shader.getID() == 0);
        REQUIRE(shader.hasCompiledSuccessfully());
    }

    SECTION("Compiles vertex shader from code") {
        REQUIRE(shader.getID() == 0);

        shader.setShader(std::string(validShaderCode));
        REQUIRE_FALSE(shader.getID() == 0);
        REQUIRE(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("Window_GLFragmentShader_setShader") { BASIL_LOCK_TEST
    GLFragmentShader shader = GLFragmentShader();

    SECTION("Compiles fragment shader from path") {
        REQUIRE(shader.getID() == 0);

        shader.setShader(fragmentPath);
        REQUIRE_FALSE(shader.getID() == 0);
        REQUIRE(shader.hasCompiledSuccessfully());
    }

    SECTION("Compiles vertex shader from code") {
        REQUIRE(shader.getID() == 0);

        shader.setShader(std::string(validShaderCode));
        REQUIRE_FALSE(shader.getID() == 0);
        REQUIRE(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("Window_GLFragmentShader_GLFragmentShader") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();
    logger.setLevel(LogLevel::DEBUG);

    SECTION("Compiles fragment shader successfully") {
        GLFragmentShader shader = GLFragmentShader(fragmentPath);

        std::string successMessage =
            "Shader compiled successfully.";

        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE_FALSE(shader.getID() == 0);
        REQUIRE(shader.hasCompiledSuccessfully());
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
            "Shader compiled successfully.";

        GLFragmentShader shader = GLFragmentShader(shaderCode);

        CHECK(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE_FALSE(shader.getID() == 0);
        REQUIRE(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("Window_GLFragmentShader_debugShader") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();
    logger.setLevel(LogLevel::DEBUG);

    std::string successMessage =
        "Shader compiled successfully.";

    SECTION("Compiles vertex shader successfully") {
        auto shader = GLFragmentShader::debugShader();

        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);

        REQUIRE_FALSE(shader->getID() == 0);
        REQUIRE(shader->hasCompiledSuccessfully());
    }
}
