#include <catch.hpp>

#include "Data/GLUniform.hpp"
#include "Data/ShaderUniformModel.hpp"

using basil::GLUniformType;
using basil::ShaderUniformModel;

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

TEST_CASE("Data_ShaderUniformModel_getUniformValue") {
    auto dataModel = ShaderUniformModel();

    SECTION("Returns nullopts if not found") {
        CHECK_FALSE(dataModel.getUniform("missing").has_value());
        CHECK_FALSE(dataModel.getUniform(-1).has_value());
    }
}
