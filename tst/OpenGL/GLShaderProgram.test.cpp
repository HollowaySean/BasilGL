#include <catch.hpp>

#include "Data/ShaderUniformModel.hpp"
#include "OpenGL/GLShaderProgram.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::DataMessage;
using basil::Logger;
using basil::LogLevel;
using basil::GLProgramUniformManager;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLTexture2D;
using basil::GLUniform;
using basil::GLUniformScalar;
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

        CHECK((unsigned int)currentID == shaderProgram.getID());
    }
}

TEST_CASE("OpenGL_GLShaderProgram_setUniform") {
    auto program = GLShaderProgram();
    auto uniform = std::make_shared<GLUniformScalar<float>>(15.5f, "myFloat");
    GLProgramUniformManager& manager = program.uniformManager;

    SECTION("Adds uniform to uniform manager") {
        CHECK(manager.uniformCache.size() == 0);
        program.setUniform(uniform);
        CHECK(manager.uniformCache.size() == 1);
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
        GLuint initialID = shaderProgram.getID();
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
}

TEST_CASE("OpenGL_GLShaderProgram_receiveData") {
    auto shaderProgram = GLShaderProgram();
    auto dataModel = ShaderUniformModel();
    GLProgramUniformManager& manager = shaderProgram.uniformManager;

    SECTION("Sets uniforms from model") {
        dataModel.addUniform(true, "myBool");
        auto message = DataMessage(dataModel);

        CHECK(manager.uniformCache.size() == 0);
        shaderProgram.receiveData(message);
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
}
