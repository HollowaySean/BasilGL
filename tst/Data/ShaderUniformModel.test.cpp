#include <catch.hpp>

#include "Data/GLUniform.hpp"
#include "Data/ShaderUniformModel.hpp"
#include "Data/SpanTextureSource.hpp"

#include "Window/GLTexture.hpp"

using basil::GLUniformType;
using basil::IGLTexture;
using basil::GLTexture1D;
using basil::GLTexture2D;
using basil::ShaderUniformModel;
using basil::SpanTextureSource;

TEST_CASE("Data_ShaderUniformModel_addUniformValue") {
    auto dataModel = ShaderUniformModel();
    dataModel.addUniformValue(true, "myBool");
    dataModel.addUniformValue(1.5f, "myFloat");
    dataModel.addUniformValue(-15, "myInt");
    dataModel.addUniformValue(2, "myUint");

    SECTION("Saves uniforms to container") {
        CHECK(dataModel.getUniforms().size() == 4);

        CHECK(dataModel.getUniform("myBool").value().value
            == GLUniformType(true));
        CHECK(dataModel.getUniform("myFloat").value().value
            == GLUniformType(1.5f));
        CHECK(dataModel.getUniform("myInt").value().value
            == GLUniformType(-15));
        CHECK(dataModel.getUniform("myUint").value().value
            == GLUniformType(2));
    }
}

TEST_CASE("Data_ShaderUniformModel_setUniformValue") {
    auto dataModel = ShaderUniformModel();
    unsigned int boolID = dataModel.addUniformValue(true, "myBool");
    unsigned int floatID = dataModel.addUniformValue(1.5f, "myFloat");
    unsigned int intID = dataModel.addUniformValue(-15, "myInt");
    unsigned int uintID = dataModel.addUniformValue(2, "myUint");

    dataModel.setUniformValue(boolID, true);
    dataModel.setUniformValue(floatID, 1.5f);
    dataModel.setUniformValue(intID, -15);
    dataModel.setUniformValue(uintID, 2);

    SECTION("Updates uniform values") {
        CHECK(dataModel.getUniform(boolID).value().value
            == GLUniformType(true));
        CHECK(dataModel.getUniform(floatID).value().value
            == GLUniformType(1.5f));
        CHECK(dataModel.getUniform(intID).value().value
            == GLUniformType(-15));
        CHECK(dataModel.getUniform(uintID).value().value
            == GLUniformType(2));
    }

    SECTION("Returns true if uniform exists") {
        CHECK(dataModel.setUniformValue(boolID, true));
    }

    SECTION("Returns false if uniform does not exist") {
        CHECK_FALSE(dataModel.setUniformValue(-1, true));
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
    auto texture = GLTexture1D::Builder()
        .fromSpan<float>(values)
        .build();

    auto dataModel = ShaderUniformModel();
    int ID = dataModel.addTexture(texture, "texture1");

    SECTION("Updates texture list") {
        auto resultFromName = dataModel.getTexture("texture1");
        CHECK(resultFromName.value().texture == texture);

        auto resultFromID = dataModel.getTexture(ID);
        CHECK(resultFromID.value().texture == texture);
    }
}

TEST_CASE("Data_ShaderUniformModel_getTexture") {
    auto dataModel = ShaderUniformModel();

    SECTION("Returns nullopts if not found") {
        auto result = dataModel.getTexture("missing");
        CHECK_FALSE(result.has_value());
    }

    SECTION("Returns nullopts if not found") {
        auto result = dataModel.getTexture(-1);
        CHECK_FALSE(result.has_value());
    }
}

TEST_CASE("Data_ShaderUniformModel_Builder") {
    std::shared_ptr<IGLTexture> texture = std::make_shared<GLTexture1D>();
    auto model = ShaderUniformModel::Builder()
        .withTexture(texture, "myTexture")
        .withUniform(1.5f, "myFloat")
        .build();

    SECTION("Builds with textures and uniforms") {
        CHECK(model->getTexture("myTexture").has_value());
        CHECK(model->getTexture("myTexture").value().texture
            == texture);

        CHECK(model->getUniform("myFloat").has_value());
        CHECK(model->getUniform("myFloat").value().value
            == GLUniformType(1.5f));
    }
}
