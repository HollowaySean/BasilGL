#include <catch.hpp>

#include "Data/ShaderUniformModel.hpp"

#include "OpenGL/SpanTextureSource.hpp"
#include "OpenGL/GLUniform.hpp"
#include "OpenGL/GLTexture.hpp"

using basil::GLUniform;
using basil::GLUniformVector;
using basil::IGLTexture;
using basil::GLTexture1D;
using basil::GLTexture2D;
using basil::ShaderUniformModel;
using basil::SpanTextureSource;

TEST_CASE("Data_ShaderUniformModel_addUniform") {
    auto dataModel = ShaderUniformModel();
    dataModel.addUniform(GLUniform(true, "myBool"));
    dataModel.addUniform(GLUniform(1.5f, "myFloat"));
    dataModel.addUniform(GLUniform(-15, "myInt"));
    dataModel.addUniform(GLUniform(5, "myUint"));
    dataModel.addUniform(GLUniform(2, "myUint"));

    SECTION("Saves uniforms to container") {
        CHECK(dataModel.getUniforms().size() == 4);

        CHECK(dataModel.getUniform("myBool").value().value
            == GLUniformVector(std::vector<bool>({true})));
        CHECK(dataModel.getUniform("myFloat").value().value
            == GLUniformVector(std::vector<float>({1.5f})));
        CHECK(dataModel.getUniform("myInt").value().value
            == GLUniformVector(std::vector<int>({-15})));
        CHECK(dataModel.getUniform("myUint").value().value
            == GLUniformVector(std::vector<int>({2})));
    }
}

TEST_CASE("Data_ShaderUniformModel_setUniformValue") {
    auto dataModel = ShaderUniformModel();
    unsigned int boolID = dataModel.addUniform(GLUniform(true, "myBool"));
    unsigned int floatID = dataModel.addUniform(GLUniform(1.5f, "myFloat"));
    unsigned int intID = dataModel.addUniform(GLUniform(-15, "myInt"));
    unsigned int uintID = dataModel.addUniform(GLUniform(2, "myUint"));

    dataModel.setUniformValue(true, boolID);
    dataModel.setUniformValue(1.5f, floatID);
    dataModel.setUniformValue(-15, intID);
    dataModel.setUniformValue(2, uintID);

    SECTION("Updates uniform values") {
        CHECK(dataModel.getUniform(boolID).value().value
            == GLUniformVector(std::vector<bool>({true})));
        CHECK(dataModel.getUniform(floatID).value().value
            == GLUniformVector(std::vector<float>({1.5f})));
        CHECK(dataModel.getUniform(intID).value().value
            == GLUniformVector(std::vector<int>({-15})));
        CHECK(dataModel.getUniform(uintID).value().value
            == GLUniformVector(std::vector<int>({2})));
    }

    SECTION("Returns true if uniform exists") {
        CHECK(dataModel.setUniformValue(true, boolID));
    }

    SECTION("Returns false if uniform does not exist") {
        CHECK_FALSE(dataModel.setUniformValue(true, -1));
    }
}

TEST_CASE("Data_ShaderUniformModel_getUniform") {
    auto dataModel = ShaderUniformModel();

    SECTION("Returns nullopts if not found") {
        CHECK_FALSE(dataModel.getUniform("missing").has_value());
        CHECK_FALSE(dataModel.getUniform(-1).has_value());
    }
}

TEST_CASE("Data_ShaderUniformModel_addTexture") {
    std::vector<float> values = { 1, 2, 3, 4 };
    auto texture1 = GLTexture1D::Builder()
        .fromSpan<float>(values)
        .build();
    auto texture2 = GLTexture1D::Builder()
        .fromSpan<float>(values)
        .build();

    auto dataModel = ShaderUniformModel();
    int ID1 = dataModel.addTexture(texture1, "texture1");
    int ID2 = dataModel.addTexture(texture2, "texture1");

    SECTION("Updates texture list") {
        CHECK(ID1 == ID2);
        CHECK(dataModel.getTextures().size() == 1);

        auto resultFromName = dataModel.getTextureSource("texture1");
        CHECK(resultFromName.value() == texture2);

        auto resultFromID = dataModel.getTextureSource(ID1);
        CHECK(resultFromID.value() == texture2);
    }
}

TEST_CASE("Data_ShaderUniformModel_setTextureSource") {
    std::vector<float> values = { 1, 2, 3, 4 };
    auto texture1 = GLTexture1D::Builder()
        .fromSpan<float>(values)
        .build();
    auto texture2 = GLTexture1D::Builder()
        .fromSpan<float>(values)
        .build();

    auto dataModel = ShaderUniformModel();

    SECTION("Updates texture source if it exists") {
        int ID = dataModel.addTexture(texture1, "texture1");
        bool didSet = dataModel.setTextureSource(texture2, ID);

        CHECK(didSet);
        CHECK(dataModel.getTextures().size() == 1);

        auto resultFromName = dataModel.getTextureSource("texture1");
        CHECK(resultFromName.value() == texture2);

        auto resultFromID = dataModel.getTextureSource(ID);
        CHECK(resultFromID.value() == texture2);
    }

    SECTION("Returns false if texture does not exist") {
        bool didSet = dataModel.setTextureSource(texture1, -1);

        CHECK_FALSE(didSet);
        CHECK(dataModel.getTextures().size() == 0);
    }
}

TEST_CASE("Data_ShaderUniformModel_getTexture") {
    auto dataModel = ShaderUniformModel();

    SECTION("Returns nullopts if not found") {
        auto result = dataModel.getTextureSource("missing");
        CHECK_FALSE(result.has_value());
    }

    SECTION("Returns nullopts if not found") {
        auto result = dataModel.getTextureSource(-1);
        CHECK_FALSE(result.has_value());
    }
}

TEST_CASE("Data_ShaderUniformModel_Builder") {
    std::shared_ptr<IGLTexture> texture = std::make_shared<GLTexture1D>();
    auto model = ShaderUniformModel::Builder()
        .withTexture(texture, "myTexture")
        .withUniform(GLUniform(1.5f, "myFloat"))
        .build();

    SECTION("Builds with textures and uniforms") {
        CHECK(model->getTextureSource("myTexture").has_value());
        CHECK(model->getTextureSource("myTexture").value()
            == texture);

        CHECK(model->getUniform("myFloat").has_value());
        CHECK(model->getUniform("myFloat").value().value
            == GLUniformVector(std::vector<float>({1.5f})));
    }
}
