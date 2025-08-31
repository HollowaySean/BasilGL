#include <sstream>

#include <catch.hpp>

#include "OpenGL/GLShader.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::GLShader;
using basil::GLVertexShader;
using basil::GLFragmentShader;

class TestShader : public GLShader {
 public:
    TestShader() = default;
    void setShader(std::filesystem::path /* path */) override {}
    void setShader(const std::string& /* shaderCode */) override {}
};

TEST_CASE("OpenGL_GLShader_getShaderFromFile") {
    TestShader shader = TestShader();

    SECTION("Reads valid file successfully") {
        shader.getShaderFromFile(validPath);

        CHECK(shader.rawShaderCode == "test-message");
        CHECK(shader.hasCompiledSuccessfully());
    }

    SECTION("Adds prefix & suffix to Shadertoy file") {
        shader.getShaderFromFile(shaderToyPath);

        CHECK(shader.rawShaderCode.starts_with(GLShader::SHADERTOY_PREFIX));
        CHECK(shader.rawShaderCode.ends_with(GLShader::SHADERTOY_SUFFIX));
    }

    SECTION("Prints error for missing file") {
        shader.getShaderFromFile(invalidPath);

        CHECK(shader.rawShaderCode == "");
        CHECK_FALSE(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("OpenGL_GLShader_compileShader") {
    TestShader shader = TestShader();

    SECTION("Compiles valid shader code successfully") {
        shader.rawShaderCode = validShaderCode;
        shader.hasCompiled = true;
        shader.compileShader(GLShader::ShaderType::FRAGMENT);

        CHECK(shader.hasCompiledSuccessfully());
    }

    SECTION("Fails to compile invalid shader code") {
        shader.rawShaderCode = "fdsjkalfbrejwkl";
        shader.hasCompiled = true;
        shader.compileShader(GLShader::ShaderType::FRAGMENT);

        CHECK_FALSE(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("OpenGL_GLVertexShader_GLVertexShader") {
    SECTION("Compiles vertex shader successfully") {
        GLVertexShader shader = GLVertexShader(vertexPath);

        CHECK_FALSE(shader.getID() == 0);
        CHECK(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("OpenGL_GLVertexShader_noOpShader") {
    std::string successMessage =
        "Shader compiled successfully.";

    SECTION("Compiles vertex shader successfully") {
        auto shader = GLVertexShader::noOpShader();

        CHECK_FALSE(shader->getID() == 0);
        CHECK(shader->hasCompiledSuccessfully());
    }
}

TEST_CASE("OpenGL_GLVertexShader_setShader") {
    GLVertexShader shader = GLVertexShader();

    SECTION("Compiles vertex shader from path") {
        CHECK(shader.getID() == 0);

        shader.setShader(vertexPath);
        CHECK_FALSE(shader.getID() == 0);
        CHECK(shader.hasCompiledSuccessfully());
    }

    SECTION("Compiles vertex shader from code") {
        CHECK(shader.getID() == 0);

        shader.setShader(std::string(validShaderCode));
        CHECK_FALSE(shader.getID() == 0);
        CHECK(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("OpenGL_GLFragmentShader_setShader") {
    GLFragmentShader shader = GLFragmentShader();

    SECTION("Compiles fragment shader from path") {
        CHECK(shader.getID() == 0);

        shader.setShader(fragmentPath);
        CHECK_FALSE(shader.getID() == 0);
        CHECK(shader.hasCompiledSuccessfully());
    }

    SECTION("Compiles vertex shader from code") {
        CHECK(shader.getID() == 0);

        shader.setShader(std::string(validShaderCode));
        CHECK_FALSE(shader.getID() == 0);
        CHECK(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("OpenGL_GLFragmentShader_GLFragmentShader") {
    SECTION("Compiles fragment shader successfully") {
        GLFragmentShader shader = GLFragmentShader(fragmentPath);

        CHECK_FALSE(shader.getID() == 0);
        CHECK(shader.hasCompiledSuccessfully());
    }

    SECTION("Compiles fragment shader from string") {
        std::string shaderCode = "#version 330 core\n"
            "uniform int myUniformInt;\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "vec2 st = vec2(gl_FragCoord.x * myUniformInt, "
                "gl_FragCoord.y / 480);\n"
                "gl_FragColor = vec4(st.x, st.y, 0.0, 1.0); }\0";

        GLFragmentShader shader = GLFragmentShader(shaderCode);

        CHECK_FALSE(shader.getID() == 0);
        CHECK(shader.hasCompiledSuccessfully());
    }
}

TEST_CASE("OpenGL_GLFragmentShader_debugShader") {
    SECTION("Compiles vertex shader successfully") {
        auto shader = GLFragmentShader::debugShader();

        CHECK_FALSE(shader->getID() == 0);
        CHECK(shader->hasCompiledSuccessfully());
    }
}
