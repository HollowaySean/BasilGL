#include <catch.hpp>

#include <Basil/Window.hpp>

using basil::Logger;
using basil::LogLevel;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;

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

TEST_CASE("Window_GLShaderProgram_GLShaderProgram") {
    Logger& logger = Logger::get();
    std::filesystem::path testPath = TEST_DIR;

    SECTION("Compiles shader program.") {
        std::filesystem::path filePath =
            std::filesystem::path(TEST_DIR) / "Window/assets/test.vert";
        std::shared_ptr<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(filePath);

        filePath =
            std::filesystem::path(TEST_DIR) / "Window/assets/test.frag";
        std::shared_ptr<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(filePath);

        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        std::string successMessage =
            "Shader program compiled successfully.";

        CHECK(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == successMessage);
    }

    SECTION("Logs error for failed compilation.") {
        std::shared_ptr<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(
                std::filesystem::path(TEST_DIR) / "Window/assets/test.vert");
        std::shared_ptr<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(
                std::filesystem::path(""));

        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
    }
}

TEST_CASE("Window_GLShaderProgram_use") {
    SECTION("Sets current program to shader program ID") {
        std::shared_ptr<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(
                std::filesystem::path(TEST_DIR) / "Window/assets/test.vert");
        std::shared_ptr<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(
                std::filesystem::path(TEST_DIR) / "Window/assets/test.frag");

        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        shaderProgram.use();

        GLint currentID;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentID);

        REQUIRE(currentID == shaderProgram.getID());
    }
}

TEST_CASE("Window_GLShaderProgram_setUniform") {
    SECTION("Sets uniform in shader program") {
        std::shared_ptr<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(
                std::filesystem::path(TEST_DIR) / "Window/assets/test.vert");
        std::shared_ptr<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(
                std::filesystem::path(TEST_DIR) / "Window/assets/test.frag");

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

TEST_CASE("Window_GLShaderProgram_setUniformVector") {
    SECTION("Sets uniform vector in shader program") {
        std::shared_ptr<GLVertexShader> vertexShader =
            std::make_shared<GLVertexShader>(
                std::filesystem::path(TEST_DIR) / "Window/assets/test.vert");
        std::shared_ptr<GLFragmentShader> fragmentShader =
            std::make_shared<GLFragmentShader>(
                std::filesystem::path(TEST_DIR) / "Window/assets/test.frag");

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
