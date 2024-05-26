#include <catch.hpp>

#include "Data/ShaderUniformModel.hpp"
#include "OpenGL/GLShaderProgram.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::DataMessage;
using basil::IGLTexture;
using basil::Logger;
using basil::LogLevel;
using basil::GLProgramUniformManager;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLTexture2D;
using basil::GLUniform;
using basil::ShaderUniformModel;

TEST_CASE("OpenGL_GLShaderProgram_GLShaderProgram") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();

    SECTION("Does not compile for default contruction") {
        GLShaderProgram program = GLShaderProgram();

        CHECK(program.getID() == 0);
        CHECK_FALSE(program.hasLinkedSuccessfully());
    }

    SECTION("Compiles shader program.") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        CHECK(logger.getLastLevel() == LogLevel::INFO);
        CHECK_FALSE(shaderProgram.getID() == 0);
        CHECK(shaderProgram.hasLinkedSuccessfully());
    }

    SECTION("Logs error for failed compilation.") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader = std::make_shared<GLFragmentShader>(
            std::string("uncompilable"));

        logger.clearTestInfo();
        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        CHECK_FALSE(shaderProgram.hasLinkedSuccessfully());
        CHECK_FALSE(shaderProgram.getID() == 0);
        CHECK(logger.getLastLevel() == LogLevel::ERROR);
    }
}

TEST_CASE("OpenGL_GLShaderProgram_use") { BASIL_LOCK_TEST
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

        CHECK(currentID == shaderProgram.getID());
    }
}

TEST_CASE("OpenGL_GLShaderProgram_setUniform") {
    auto program = GLShaderProgram();
    auto uniform = GLUniform(15.5f, "myFloat");
    GLProgramUniformManager& manager = program.uniformManager;

    SECTION("Adds uniform to uniform manager") {
        CHECK(manager.uniformCache.size() == 0);
        program.setUniform(uniform);
        CHECK(manager.uniformCache.size() == 1);
    }
}

TEST_CASE("OpenGL_GLShaderProgram_setScalarUniform") {
    auto program = GLShaderProgram();
    GLProgramUniformManager& manager = program.uniformManager;

    SECTION("Adds uniform to uniform manager") {
        CHECK(manager.uniformCache.size() == 0);
        program.setScalarUniform(15.5f, "myFloat");
        CHECK(manager.uniformCache.size() == 1);
    }
}

TEST_CASE("OpenGL_GLShaderProgram_setTextureUniform") {
    auto program = GLShaderProgram();
    auto texture = std::make_shared<GLTexture2D>();
    GLProgramUniformManager& manager = program.uniformManager;

    SECTION("Adds texture to uniform manager") {
        CHECK(manager.textureMap.size() == 0);
        program.setTextureUniform(texture, "testTex");
        CHECK(manager.textureMap.size() == 1);
    }
}

TEST_CASE("OpenGL_GLShaderProgram_updateShaders") { BASIL_LOCK_TEST
    auto vertexShader =
        std::make_shared<GLVertexShader>(vertexPath);
    auto secondVertexShader =
        std::make_shared<GLVertexShader>(vertexPath);
    auto fragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);
    auto secondFragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);


    SECTION("Only compiles when both shaders are present") {
        GLShaderProgram shaderProgram = GLShaderProgram();
        CHECK(shaderProgram.getID() == 0);
        CHECK_FALSE(shaderProgram.hasLinkedSuccessfully());

        shaderProgram.setFragmentShader(fragmentShader);
        CHECK(shaderProgram.getID() == 0);
        CHECK_FALSE(shaderProgram.hasLinkedSuccessfully());

        shaderProgram.setVertexShader(vertexShader);
        CHECK_FALSE(shaderProgram.getID() == 0);
        CHECK(shaderProgram.hasLinkedSuccessfully());
    }

    SECTION("Re-compiles when shader is replaced, if not null") {
        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);
        GLint initialID = shaderProgram.getID();
        CHECK(initialID != 0);
        CHECK(shaderProgram.hasLinkedSuccessfully());

        shaderProgram.setFragmentShader(nullptr);
        CHECK(shaderProgram.getID() == initialID);
        CHECK(shaderProgram.hasLinkedSuccessfully());
        CHECK(shaderProgram.getFragmentShader() == fragmentShader);

        shaderProgram.setFragmentShader(secondFragmentShader);
        CHECK(shaderProgram.getID() == initialID);
        CHECK(shaderProgram.hasLinkedSuccessfully());
        CHECK(shaderProgram.getFragmentShader() == secondFragmentShader);

        shaderProgram.setVertexShader(secondVertexShader);
        CHECK(shaderProgram.getID() == initialID);
        CHECK(shaderProgram.hasLinkedSuccessfully());
        CHECK(shaderProgram.getVertexShader() == secondVertexShader);
    }

    // SECTION("Re-applies cached uniforms") {
    //     GLShaderProgram shaderProgram =
    //         GLShaderProgram(vertexShader, fragmentShader);
    //     shaderProgram.setUniform("myUniformInt", 128);

    //     GLint location = glGetUniformLocation(
    //         shaderProgram.getID(), "myUniformInt");
    //     GLint initial;
    //     glGetUniformiv(shaderProgram.getID(), location, &initial);
    //     CHECK(initial == 128);

    //     shaderProgram.setFragmentShader(secondFragmentShader);
    //     CHECK(shaderProgram.getFragmentShader() == secondFragmentShader);

    //     location = glGetUniformLocation(
    //         shaderProgram.getID(), "myUniformInt");
    //     GLint result;
    //     glGetUniformiv(shaderProgram.getID(), location, &result);
    //     CHECK(result == 128);
    // }
}

// TEST_CASE("OpenGL_GLShaderProgram_getUniformLocation") { BASIL_LOCK_TEST
//     auto program = GLShaderProgram::Builder()
//         .withFragmentShaderFromFile(fragmentPath)
//         .withDefaultVertexShader()
//         .build();

//     SECTION("Returns location of existing uniform") {
//         GLint location = program->getUniformLocation("myUniformBool");
//         CHECK(location >= 0);
//     }

//     SECTION("Returns -1 and logs missing uniform") {
//         Logger& logger = Logger::get();

//         GLint location = program->getUniformLocation("missingUniform");
//         CHECK(location == -1);
//         CHECK(logger.getLastLevel() == LogLevel::DEBUG);
//     }
// }

// TEST_CASE("OpenGL_GLShaderProgram_setUniform") { BASIL_LOCK_TEST
//     SECTION("Sets uniform in shader program") {
//         auto vertexShader =
//             std::make_shared<GLVertexShader>(vertexPath);
//         auto fragmentShader =
//             std::make_shared<GLFragmentShader>(fragmentPath);

//         GLShaderProgram shaderProgram =
//             GLShaderProgram(vertexShader, fragmentShader);
//         shaderProgram.use();

//         verifyUniforms<bool,  1>(shaderProgram,
//             "myUniformBool", testBool);
//         verifyUniforms<int,   1>(shaderProgram,
//             "myUniformInt",  testInt);
//         verifyUniforms<uint,  1>(shaderProgram,
//             "myUniformUnsignedInt", testUint);
//         verifyUniforms<float, 1>(shaderProgram,
//             "myUniformFloat", testFloat);
//     }
// }

// TEST_CASE("OpenGL_GLShaderProgram_setUniformVector") { BASIL_LOCK_TEST
//     SECTION("Sets uniform vector in shader program") {
//         auto vertexShader =
//             std::make_shared<GLVertexShader>(vertexPath);
//         auto fragmentShader =
//             std::make_shared<GLFragmentShader>(fragmentPath);

//         GLShaderProgram shaderProgram =
//             GLShaderProgram(vertexShader, fragmentShader);
//         shaderProgram.use();

//         verifyUniforms<bool,  2>(shaderProgram,
//             "myUniformBool2", testBool);
//         verifyUniforms<int,   2>(shaderProgram,
//             "myUniformInt2",  testInt);
//         verifyUniforms<uint,  2>(shaderProgram,
//             "myUniformUnsignedInt2", testUint);
//         verifyUniforms<float, 2>(shaderProgram,
//             "myUniformFloat2", testFloat);

//         verifyUniforms<bool,  3>(shaderProgram,
//             "myUniformBool3", testBool);
//         verifyUniforms<int,   3>(shaderProgram,
//             "myUniformInt3",  testInt);
//         verifyUniforms<uint,  3>(shaderProgram,
//             "myUniformUnsignedInt3", testUint);
//         verifyUniforms<float, 3>(shaderProgram,
//             "myUniformFloat3", testFloat);

//         verifyUniforms<bool,  4>(shaderProgram,
//             "myUniformBool4", testBool);
//         verifyUniforms<int,   4>(shaderProgram,
//             "myUniformInt4",  testInt);
//         verifyUniforms<uint,  4>(shaderProgram,
//             "myUniformUnsignedInt4", testUint);
//         verifyUniforms<float, 4>(shaderProgram,
//             "myUniformFloat4", testFloat);
//     }
// }

// template<class T>
// void verifyDataModelScalar(
//         GLShaderProgram* program, ShaderUniformModel* dataModel,
//         const std::string& name, T value) {
//     dataModel->addUniformValue(value, name);
//     auto message = DataMessage(*dataModel);
//     program->receiveData(message);

//     GLint location =
//         glGetUniformLocation(program->getID(), name.c_str());
//     T result;
//     getUniform<1>(program->getID(), location, &result);
//     CHECK(result == value);
// }

// template<class T, int N>
// void verifyDataModelVector(
//         GLShaderProgram* program, ShaderUniformModel* dataModel,
//         const std::string& name, T values[N]) {
//     std::vector<T> input = std::vector<T>();
//     for (int i = 0; i < N; i++) {
//         input.push_back(values[i]);
//     }

//     dataModel->addUniformValue(input, name);
//     auto message = DataMessage(*dataModel);
//     program->receiveData(message);

//     GLint location =
//         glGetUniformLocation(program->getID(), name.c_str());
//     T result[N];
//     getUniform<N>(program->getID(), location, result);

//     for (int i = 0; i < N; i++) {
//         CHECK(result[i] == values[i]);
//     }
// }

TEST_CASE("OpenGL_GLShaderProgram_receiveData") {
    auto shaderProgram = GLShaderProgram();
    auto dataModel = ShaderUniformModel();
    GLProgramUniformManager& manager = shaderProgram.uniformManager;

    SECTION("Sets uniforms from model") {
        dataModel.addUniform(GLUniform(true, "myBool"));
        auto message = DataMessage(dataModel);

        CHECK(manager.uniformCache.size() == 0);
        shaderProgram.receiveData(message);
        CHECK(manager.uniformCache.size() == 1);
    }

    SECTION("Sets textures from model") {
        auto texture = std::make_shared<GLTexture2D>();

        dataModel.addTexture(texture, "myTexture");
        auto message = DataMessage(dataModel);

        CHECK(manager.textureMap.size() == 0);
        CHECK(manager.uniformCache.size() == 0);
        shaderProgram.receiveData(message);
        CHECK(manager.textureMap.size() == 1);
        CHECK(manager.uniformCache.size() == 1);
    }
}

TEST_CASE("OpenGL_GLShaderProgram_Builder") { BASIL_LOCK_TEST
    SECTION("Builds shaders from objects") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        auto program = GLShaderProgram::Builder()
            .withFragmentShader(fragmentShader)
            .withVertexShader(vertexShader)
            .build();

        CHECK(program->fragmentShader == fragmentShader);
        CHECK(program->vertexShader == vertexShader);
        CHECK_FALSE(program->getID() == 0);

        program = GLShaderProgram::Builder()
            .withDefaultVertexShader()
            .build();

        CHECK(program->fragmentShader == nullptr);
        CHECK_FALSE(program->vertexShader == nullptr);
        CHECK(program->vertexShader->rawShaderCode
            == basil::GLShader::NO_OP_VERTEX_CODE);
        CHECK(program->getID() == 0);
    }

    SECTION("Builds shaders from file") {
        auto program = GLShaderProgram::Builder()
            .withFragmentShaderFromFile(fragmentPath)
            .withVertexShaderFromFile(vertexPath)
            .build();

        CHECK_FALSE(program->fragmentShader == nullptr);
        CHECK_FALSE(program->vertexShader == nullptr);
        CHECK_FALSE(program->getID() == 0);

        program = GLShaderProgram::Builder()
            .withFragmentShaderFromCode(program->fragmentShader->rawShaderCode)
            .withVertexShaderFromCode(program->vertexShader->rawShaderCode)
            .build();

        CHECK_FALSE(program->fragmentShader == nullptr);
        CHECK_FALSE(program->vertexShader == nullptr);
        CHECK_FALSE(program->getID() == 0);
    }

    // SECTION("Sets uniforms") {
    //     auto program = GLShaderProgram::Builder()
    //         .withDefaultVertexShader()
    //         .withFragmentShaderFromFile(fragmentPath)
    //         .withUniform("myUniformBool", testBool[0])
    //         .withUniform("myUniformInt", testInt[0])
    //         .withUniform("myUniformUnsignedInt", testUint[0])
    //         .withUniform("myUniformFloat", testFloat[0])
    //         .withUniformVector("myUniformBool2",
    //             std::vector({ testBool[0], testBool[1] }))
    //         .build();


    //     CHECK(glGetUniformLocation(program->getID(),
    //         "myUniformBool") >= 0);
    //     CHECK(glGetUniformLocation(program->getID(),
    //         "myUniformInt") >= 0);
    //     CHECK(glGetUniformLocation(program->getID(),
    //         "myUniformUnsignedInt") >= 0);
    //     CHECK(glGetUniformLocation(program->getID(),
    //         "myUniformFloat") >= 0);
    // }

    // SECTION("Adds texture") {
    //     std::vector<float> span = { 0.0, 1.0, 2.0, 3.0 };
    //     auto program = GLShaderProgram::Builder()
    //         .withDefaultVertexShader()
    //         .withFragmentShaderFromFile(fragmentPath)
    //         .withTexture("testTex", GLTexture2D::Builder()
    //             .fromSpan(std::span(span))
    //             .build())
    //         .build();


    //     CHECK(glGetUniformLocation(program->getID(), "testTex") >= 0);
    // }
}
