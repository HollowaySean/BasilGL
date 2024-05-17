#include <catch.hpp>

#include "Data/ShaderUniformModel.hpp"
#include "Window/GLShaderProgram.hpp"

#include "Window/GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::DataMessage;
using basil::IGLTexture;
using basil::Logger;
using basil::LogLevel;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLTexture2D;
using basil::ShaderUniformModel;

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
            shaderProgram.setUniform(name,
                values[0], values[1]);
            break;
        case 3:
            shaderProgram.setUniform(name,
                values[0], values[1], values[2]);
            break;
        case 4:
            shaderProgram.setUniform(name,
                values[0], values[1], values[2], values[3]);
            break;
    }

    GLint location =
        glGetUniformLocation(shaderProgram.getID(), name.c_str());

    T setValue[N];
    getUniform<N>(shaderProgram.getID(), location, setValue);

    for (int i = 0; i < N; i++) {
        CHECK(setValue[i] == values[i]);
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

        CHECK(currentID == shaderProgram.getID());
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

        CHECK(result == texture->getUniformLocation());
    }
}

TEST_CASE("Window_GLShaderProgram_updateShaders") { BASIL_LOCK_TEST
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

    SECTION("Re-applies cached uniforms") {
        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);
        shaderProgram.setUniform("myUniformInt", 128);

        GLint location = glGetUniformLocation(
            shaderProgram.getID(), "myUniformInt");
        GLint initial;
        glGetUniformiv(shaderProgram.getID(), location, &initial);
        CHECK(initial == 128);

        shaderProgram.setFragmentShader(secondFragmentShader);
        CHECK(shaderProgram.getFragmentShader() == secondFragmentShader);

        location = glGetUniformLocation(
            shaderProgram.getID(), "myUniformInt");
        GLint result;
        glGetUniformiv(shaderProgram.getID(), location, &result);
        CHECK(result == 128);
    }
}

TEST_CASE("Window_GLShaderProgram_getUniformLocation") { BASIL_LOCK_TEST
    auto program = GLShaderProgram::Builder()
        .withFragmentShaderFromFile(fragmentPath)
        .withDefaultVertexShader()
        .build();

    SECTION("Returns location of existing uniform") {
        GLint location = program->getUniformLocation("myUniformBool");
        CHECK(location >= 0);
    }

    SECTION("Returns -1 and logs missing uniform") {
        Logger& logger = Logger::get();

        GLint location = program->getUniformLocation("missingUniform");
        CHECK(location == -1);
        CHECK(logger.getLastLevel() == LogLevel::DEBUG);
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

template<class T>
void verifyDataModelScalar(
        GLShaderProgram* program, ShaderUniformModel* dataModel,
        const std::string& name, T value) {
    dataModel->addUniformValue(value, name);
    auto message = DataMessage(*dataModel);
    program->receiveData(message);

    GLint location =
        glGetUniformLocation(program->getID(), name.c_str());
    T result;
    getUniform<1>(program->getID(), location, &result);
    CHECK(result == value);
}

template<class T, int N>
void verifyDataModelVector(
        GLShaderProgram* program, ShaderUniformModel* dataModel,
        const std::string& name, T values[N]) {
    std::vector<T> input = std::vector<T>();
    for (int i = 0; i < N; i++) {
        input.push_back(values[i]);
    }

    dataModel->addUniformValue(input, name);
    auto message = DataMessage(*dataModel);
    program->receiveData(message);

    GLint location =
        glGetUniformLocation(program->getID(), name.c_str());
    T result[N];
    getUniform<N>(program->getID(), location, result);

    for (int i = 0; i < N; i++) {
        CHECK(result[i] == values[i]);
    }
}

TEST_CASE("Window_GLShaderProgram_receiveData") { BASIL_LOCK_TEST
    SECTION("Sets uniforms from model") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);

        auto texture = std::make_shared<GLTexture2D>();

        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);
        shaderProgram.use();

        auto dataModel = ShaderUniformModel();

        verifyDataModelScalar(&shaderProgram, &dataModel,
            "myUniformBool", true);
        verifyDataModelScalar(&shaderProgram, &dataModel,
            "myUniformFloat", 1.5f);
        verifyDataModelScalar(&shaderProgram, &dataModel,
            "myUniformInt", -15);
        verifyDataModelScalar(&shaderProgram, &dataModel,
            "myUniformUnsignedInt", static_cast<uint>(2));

        verifyDataModelVector<bool,  1>(&shaderProgram, &dataModel,
            "myUniformBool", testBool);
        verifyDataModelVector<int,   1>(&shaderProgram, &dataModel,
            "myUniformInt",  testInt);
        verifyDataModelVector<uint,  1>(&shaderProgram, &dataModel,
            "myUniformUnsignedInt", testUint);
        verifyDataModelVector<float, 1>(&shaderProgram, &dataModel,
            "myUniformFloat", testFloat);

        verifyDataModelVector<bool,  2>(&shaderProgram, &dataModel,
            "myUniformBool2", testBool);
        verifyDataModelVector<int,   2>(&shaderProgram, &dataModel,
            "myUniformInt2",  testInt);
        verifyDataModelVector<uint,  2>(&shaderProgram, &dataModel,
            "myUniformUnsignedInt2", testUint);
        verifyDataModelVector<float, 2>(&shaderProgram, &dataModel,
            "myUniformFloat2", testFloat);

        verifyDataModelVector<bool,  3>(&shaderProgram, &dataModel,
            "myUniformBool3", testBool);
        verifyDataModelVector<int,   3>(&shaderProgram, &dataModel,
            "myUniformInt3",  testInt);
        verifyDataModelVector<uint,  3>(&shaderProgram, &dataModel,
            "myUniformUnsignedInt3", testUint);
        verifyDataModelVector<float, 3>(&shaderProgram, &dataModel,
            "myUniformFloat3", testFloat);

        verifyDataModelVector<bool,  4>(&shaderProgram, &dataModel,
            "myUniformBool4", testBool);
        verifyDataModelVector<int,   4>(&shaderProgram, &dataModel,
            "myUniformInt4",  testInt);
        verifyDataModelVector<uint,  4>(&shaderProgram, &dataModel,
            "myUniformUnsignedInt4", testUint);
        verifyDataModelVector<float, 4>(&shaderProgram, &dataModel,
            "myUniformFloat4", testFloat);
    }

    SECTION("Sets textures from model") {
        auto vertexShader =
            std::make_shared<GLVertexShader>(vertexPath);
        auto fragmentShader =
            std::make_shared<GLFragmentShader>(fragmentPath);
        GLShaderProgram shaderProgram =
            GLShaderProgram(vertexShader, fragmentShader);

        auto model = ShaderUniformModel();
        std::shared_ptr<IGLTexture> texture = std::make_shared<GLTexture2D>();
        model.addTexture(texture, "testTex");
        auto message = DataMessage(model);

        shaderProgram.receiveData(message);
        CHECK(shaderProgram.textures.at(0) == texture);
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

    SECTION("Sets uniforms") {
        auto program = GLShaderProgram::Builder()
            .withDefaultVertexShader()
            .withFragmentShaderFromFile(fragmentPath)
            .withUniform("myUniformBool", testBool[0])
            .withUniform("myUniformInt", testInt[0])
            .withUniform("myUniformUnsignedInt", testUint[0])
            .withUniform("myUniformFloat", testFloat[0])
            .withUniformVector("myUniformBool2",
                std::vector({ testBool[0], testBool[1] }))
            .build();


        CHECK(glGetUniformLocation(program->getID(),
            "myUniformBool") >= 0);
        CHECK(glGetUniformLocation(program->getID(),
            "myUniformInt") >= 0);
        CHECK(glGetUniformLocation(program->getID(),
            "myUniformUnsignedInt") >= 0);
        CHECK(glGetUniformLocation(program->getID(),
            "myUniformFloat") >= 0);
    }

    SECTION("Adds texture") {
        std::vector<float> span = { 0.0, 1.0, 2.0, 3.0 };
        auto program = GLShaderProgram::Builder()
            .withDefaultVertexShader()
            .withFragmentShaderFromFile(fragmentPath)
            .withTexture("testTex", GLTexture2D::Builder()
                .fromSpan(std::span(span))
                .build())
            .build();


        CHECK(glGetUniformLocation(program->getID(), "testTex") >= 0);
    }
}
