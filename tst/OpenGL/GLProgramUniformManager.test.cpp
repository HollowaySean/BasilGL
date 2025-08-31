#include <fmt/format.h>

#include <catch.hpp>

#include "OpenGL/GLProgramUniformManager.hpp"

#include "OpenGL/GLShaderProgram.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::GLFragmentShader;
using basil::GLProgramUniformManager;
using basil::GLShaderProgram;
using basil::GLTexture2D;
using basil::GLUniform;
using basil::GLUniformScalar;
using basil::GLUniformVector;

template<class T>
struct UniformTestData {
};

template<>
struct UniformTestData<float> {
    std::vector<float> data = { 0, 1, 2, 3, 4, 5, 6, 7,
        8, 9, 10, 11, 12, 13, 14, 15 };
    std::string uniformName = "myFloat{}{}{}";
};

template<>
struct UniformTestData<int> {
    std::vector<int> data = { 0, 1, 2, 3 };
    std::string uniformName = "myInt{}{}{}";
};

template<>
struct UniformTestData<unsigned int> {
    std::vector<unsigned int> data = { 0, 1, 2, 3 };
    std::string uniformName = "myUnsignedInt{}{}{}";
};

template<>
struct UniformTestData<bool> {
    std::vector<bool> data = { true, false, true, false };
    std::string uniformName = "myBool{}{}{}";
};

std::map<int, std::pair<int, int>> caseMap = {
    {1,  {1, 1}},
    {2,  {1, 2}},
    {3,  {1, 3}},
    {4,  {1, 4}},
    {5,  {2, 2}},
    {6,  {2, 3}},
    {7,  {2, 4}},
    {8,  {3, 3}},
    {9,  {3, 4}},
    {10, {4, 4}},
};

template <class T>
void getUniform(int programID, int location, T* result);

template<>
void getUniform(int programID, int location, float* result) {
    glGetUniformfv(programID, location, result);
}

template<>
void getUniform(int programID, int location, bool* result) {
    int intResult[4];
    glGetUniformiv(programID, location, intResult);
    for (int i = 0; i < 4; i++) {
        result[i] = static_cast<bool>(intResult[i]);
    }
}

template<>
void getUniform(int programID, int location, int* result) {
    glGetUniformiv(programID, location, result);
}

template<>
void getUniform(int programID, int location, unsigned int* result) {
    glGetUniformuiv(programID, location, result);
}

template<class T>
void verifyUniforms(GLProgramUniformManager manager, int testCase) {
    auto testCaseParams = caseMap.at(testCase);
    auto [width, length] = testCaseParams;
    int stride = width * length;

    auto testData = UniformTestData<T>();
    std::string name;
    if (width > 1) {
        name = fmt::format(
            fmt::runtime(testData.uniformName), width, "x", length);
    } else {
        name = fmt::format(
            fmt::runtime(testData.uniformName), length, "", "");
    }

    auto vector = std::make_shared<GLUniformVector<T>>(
        testData.data, name, length, width, 1);
    manager.setUniform(vector);

    GLint location =
        glGetUniformLocation(manager.programID, name.c_str());
    T result[16];
    getUniform(manager.programID, location, result);

    for (int n = 0; n < stride; n++) {
        CHECK(result[n] == testData.data.at(n));
    }
}

TEST_CASE("OpenGL_GLProgramUniformManager_setUniform") {
    auto program = GLShaderProgram::Builder()
        .withFragmentShaderFromFile(fragmentPath)
        .withDefaultVertexShader()
        .build();
    const auto& manager = program->uniformManager;

    SECTION("Sets uniforms in shader") {
        for (int length = 1; length <= 4; length++) {
            verifyUniforms<float        >(manager, length);
            verifyUniforms<int          >(manager, length);
            verifyUniforms<unsigned int >(manager, length);
            verifyUniforms<bool         >(manager, length);
        }

        for (std::size_t testCase = 5; testCase <= caseMap.size(); testCase++) {
            verifyUniforms<float>(manager, testCase);
        }
    }
}

TEST_CASE("OpenGL_GLProgramUniformManager_getUniformLocation") {
    auto program = GLShaderProgram::Builder()
        .withFragmentShaderFromFile(fragmentPath)
        .withDefaultVertexShader()
        .build();
    auto& manager = program->uniformManager;

    SECTION("Returns -1 if uniform not present in shader") {
        int location = manager.getUniformLocation("myFloap1");
        CHECK(location == -1);
    }
}

TEST_CASE("OpenGL_GLProgramUniformManager_applyCachedUniforms") {
    auto program = GLShaderProgram::Builder()
        .withFragmentShaderFromFile(fragmentPath)
        .withDefaultVertexShader()
        .build();
    auto& manager = program->uniformManager;
    std::string name = "myInt1";

    std::shared_ptr<GLUniform> uniform1 =
        std::make_shared<GLUniformScalar<int>>(1, name);
    std::shared_ptr<GLUniform> uniform2 =
        std::make_shared<GLUniformScalar<int>>(2, name);

    SECTION("Updates uniforms using most recent cached version") {
        manager.setUniform(uniform1);
        manager.setUniform(uniform2);

        manager.applyCachedUniforms();

        GLint location =
            glGetUniformLocation(manager.programID, name.c_str());
        int result[1];
        glGetUniformiv(manager.programID, location, result);

        CHECK(result[0] == 2);
    }
}
