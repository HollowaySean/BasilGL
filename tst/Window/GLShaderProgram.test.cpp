#include <catch.hpp>

#include <Basil/Window.hpp>

#include "GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::Logger;
using basil::LogLevel;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLTexture2D;

bool    testBool[4]  =  { true, false, true, true };
int     testInt[4]   =  { -10, 0, 10, 55 };
uint    testUint[4]  =  { 5, 87, 0, 1 };
float   testFloat[4] =  { 1.5, 10, 1.876, 0 };

template<class T, unsigned int N>
void verifyUniforms(GLShaderProgram shaderProgram,
        const std::string& name, T values[N]) {
    switch (N) {
        case 1:
            shaderProgram.setUniform(name, values[0]);
            break;
        case 2:
            shaderProgram.setUniformVector(name,
                values[0], values[1]);
            break;
        case 3:
            shaderProgram.setUniformVector(name,
                values[0], values[1], values[2]);
            break;
        case 4:
            shaderProgram.setUniformVector(name,
                values[0], values[1], values[2], values[3]);
            break;
    }

    GLint location =
        glGetUniformLocation(shaderProgram.getID(), name.c_str());

    T setValue[N];
    getUniform<N>(shaderProgram.getID(), location, setValue);

    for (int i = 0; i < N; i++) {
        REQUIRE(setValue[i] == values[i]);
    }
}

template<unsigned int N> void getUniform(
        GLint ID, GLint location, bool* returnValue) {
    GLint returnValueInt[N];
    glGetUniformiv(ID, location, returnValueInt);
    for (int i = 0; i < N; i++) {
        returnValue[i] = static_cast<bool>(returnValueInt[i]);
    }
}

template<unsigned int N> void getUniform(
        GLint ID, GLint location, int* returnValue) {
    glGetUniformiv(ID, location, returnValue);
}

template<unsigned int N> void getUniform(
        GLint ID, GLint location, uint* returnValue) {
    glGetUniformuiv(ID, location, returnValue);
}

template<unsigned int N> void getUniform(
        GLint ID, GLint location, float* returnValue) {
    glGetUniformfv(ID, location, returnValue);
}

TEST_CASE("Window_GLShaderProgram_GLShaderProgram") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();

    std::string successMessage =
        "Shader program compiled successfully.";

    SECTION("Does not compile for default contruction") {
        GLShaderProgram program = GLShaderProgram();

        CHECK(logger.getLastOutput() != successMessage);
    }

    SECTION("Compiles shader program.") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        CHECK(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);
    }

    SECTION("Logs error for failed compilation.") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader = std::make_shared<GLFragmentShader>(
            std::string("uncompilable"));

        logger.clearTestInfo();
        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        // Test is flaky
        CHECK_NOFAIL(logger.getLastLevel() == LogLevel::ERROR);
    }
}

TEST_CASE("Window_GLShaderProgram_use") { BASIL_LOCK_TEST
    auto vertexShader =
        std::make_shared<GLVertexShader>(vertexPath);
    auto fragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);

    SECTION("Sets current program to shader program ID") {
        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        shaderProgram.use();

        GLint currentID;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentID);

        REQUIRE(currentID == shaderProgram.getID());
    }
}

TEST_CASE("Window_GLShaderProgram_addTexture") { BASIL_LOCK_TEST
    auto vertexShader =
        std::make_shared<GLVertexShader>(vertexPath);
    auto fragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);
    auto program =
        GLShaderProgram(vertexShader, fragmentShader);

    auto texture = std::make_shared<GLTexture2D>();

    SECTION("Sets correct uniform location") {
        program.addTexture("testTex", texture);

        GLint location = glGetUniformLocation(program.getID(), "testTex");

        GLint result;
        glGetUniformiv(program.getID(), location, &result);

        REQUIRE(result == texture->getUniformLocation());
    }
}

TEST_CASE("Window_GLShaderProgram_updateShaders") { BASIL_LOCK_TEST
    auto vertexShader =
        std::make_shared<GLVertexShader>(vertexPath);
    auto fragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);
    auto secondFragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);


    SECTION("Only compiles when both shaders are present") {
        GLShaderProgram shaderProgram = GLShaderProgram();
        REQUIRE(shaderProgram.getID() == 0);

        shaderProgram.setFragmentShader(fragmentShader);
        REQUIRE(shaderProgram.getID() == 0);

        shaderProgram.setVertexShader(vertexShader);
        REQUIRE_FALSE(shaderProgram.getID() == 0);
    }

    SECTION("Re-compiles when shader is replaced") {
        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);
        REQUIRE_FALSE(shaderProgram.getID() == 0);

        shaderProgram.setFragmentShader(nullptr);
        REQUIRE(shaderProgram.getID() == 0);

        shaderProgram.setFragmentShader(secondFragmentShader);
        REQUIRE_FALSE(shaderProgram.getID() == 0);
    }
}

TEST_CASE("Window_GLShaderProgram_setUniform") { BASIL_LOCK_TEST
    SECTION("Sets uniform in shader program") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);
        shaderProgram.use();

        verifyUniforms<bool,  1>(shaderProgram,
            "myUniformBool", testBool);
        verifyUniforms<int,   1>(shaderProgram,
            "myUniformInt",  testInt);
        verifyUniforms<uint,  1>(shaderProgram,
            "myUniformUnsignedInt", testUint);
        verifyUniforms<float, 1>(shaderProgram,
            "myUniformFloat", testFloat);
    }
}

TEST_CASE("Window_GLShaderProgram_setUniformVector") { BASIL_LOCK_TEST
    SECTION("Sets uniform vector in shader program") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);
        shaderProgram.use();

        verifyUniforms<bool,  2>(shaderProgram,
            "myUniformBool2", testBool);
        verifyUniforms<int,   2>(shaderProgram,
            "myUniformInt2",  testInt);
        verifyUniforms<uint,  2>(shaderProgram,
            "myUniformUnsignedInt2", testUint);
        verifyUniforms<float, 2>(shaderProgram,
            "myUniformFloat2", testFloat);

        verifyUniforms<bool,  3>(shaderProgram,
            "myUniformBool3", testBool);
        verifyUniforms<int,   3>(shaderProgram,
            "myUniformInt3",  testInt);
        verifyUniforms<uint,  3>(shaderProgram,
            "myUniformUnsignedInt3", testUint);
        verifyUniforms<float, 3>(shaderProgram,
            "myUniformFloat3", testFloat);

        verifyUniforms<bool,  4>(shaderProgram,
            "myUniformBool4", testBool);
        verifyUniforms<int,   4>(shaderProgram,
            "myUniformInt4",  testInt);
        verifyUniforms<uint,  4>(shaderProgram,
            "myUniformUnsignedInt4", testUint);
        verifyUniforms<float, 4>(shaderProgram,
            "myUniformFloat4", testFloat);
    }
}

TEST_CASE("Window_GLShaderProgram_Builder") { BASIL_LOCK_TEST
    SECTION("Builds shaders from objects") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        auto program = GLShaderProgram::Builder()
            .withFragmentShader(fragmentShader)
            .withVertexShader(vertexShader)
            .build();

        REQUIRE(program->fragmentShader == fragmentShader);
        REQUIRE(program->vertexShader == vertexShader);
        REQUIRE_FALSE(program->getID() == 0);

        program = GLShaderProgram::Builder()
            .withDefaultVertexShader()
            .build();

        REQUIRE(program->fragmentShader == nullptr);
        REQUIRE_FALSE(program->vertexShader == nullptr);
        REQUIRE(program->vertexShader->rawShaderCode
            == basil::GLShader::noOpVertexCode);
        REQUIRE(program->getID() == 0);
    }

    SECTION("Builds shaders from file") {
        auto program = GLShaderProgram::Builder()
            .withFragmentShaderFromFile(fragmentPath)
            .withVertexShaderFromFile(vertexPath)
            .build();

        REQUIRE_FALSE(program->fragmentShader == nullptr);
        REQUIRE_FALSE(program->vertexShader == nullptr);
        REQUIRE_FALSE(program->getID() == 0);

        program = GLShaderProgram::Builder()
            .withFragmentShaderFromCode(program->fragmentShader->rawShaderCode)
            .withVertexShaderFromCode(program->vertexShader->rawShaderCode)
            .build();

        REQUIRE_FALSE(program->fragmentShader == nullptr);
        REQUIRE_FALSE(program->vertexShader == nullptr);
        REQUIRE_FALSE(program->getID() == 0);
    }
}
