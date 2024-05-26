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

template<class T>
struct UniformTestData {
    std::vector<T> data;
    std::string uniformName;
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
    auto [width, length] = caseMap.at(testCase);
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

    manager.setUniform(GLUniform(testData.data, name, 1, length, width));

    GLint location =
        glGetUniformLocation(manager.programID, name.c_str());
    T result[16];
    getUniform(manager.programID, location, result);

    for (int n = 0; n < stride; n++) {
        CHECK(result[n] == testData.data.at(n));
    }
}

TEST_CASE("OpenGL_GLProgramUniformManager_setUniform") { BASIL_LOCK_TEST
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

        for (int testCase = 5; testCase <= caseMap.size(); testCase++) {
            verifyUniforms<float>(manager, testCase);
        }
    }
}

TEST_CASE("OpenGL_GLProgramUniformManager_applyCachedUniforms") {
    auto program = GLShaderProgram::Builder()
        .withFragmentShaderFromFile(fragmentPath)
        .withDefaultVertexShader()
        .build();
    auto& manager = program->uniformManager;
    std::string name = "myInt1";

    auto uniform1 = GLUniform(1, name);
    auto uniform2 = GLUniform(2, name);

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

TEST_CASE("OpenGL_GLProgramUniformManager_setTextureSource") {
    auto program = GLShaderProgram::Builder()
        .withFragmentShaderFromFile(fragmentPath)
        .withDefaultVertexShader()
        .build();
    auto& manager = program->uniformManager;
    std::string name = "testTex";

    auto texture1 = std::make_shared<GLTexture2D>();
    auto uniform1 = GLUniform(texture1, name);

    auto texture2 = std::make_shared<GLTexture2D>();
    auto uniform2 = GLUniform(texture2, name);


    SECTION("Sets or updates textures") {
        manager.setTextureSource(texture1, uniform1);

        CHECK(manager.textureMap.size() == 1);
        CHECK(manager.textureMap.at(name) == texture1);

        manager.setTextureSource(texture2, uniform2);

        CHECK(manager.textureMap.size() == 1);
        CHECK(manager.textureMap.at(name) == texture2);
    }
}
