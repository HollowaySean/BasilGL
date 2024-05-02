#include <catch.hpp>

#include <Basil/Data.hpp>

using basil::ShaderUniformModel;

TEST_CASE("Data_ShaderUniformModel_setUniformValue") {
    auto dataModel = ShaderUniformModel();
    dataModel.setUniformValue(true, "myBool");
    dataModel.setUniformValue(1.5f, "myFloat");
    dataModel.setUniformValue(-15, "myInt");
    dataModel.setUniformValue(2, "myUint");

    SECTION("Saves uniforms to container") {
        REQUIRE(dataModel.getUniforms().size() == 4);

        REQUIRE(dataModel.getUniformValue("myBool").value()
            == ShaderUniformModel::GLUniformType(true));
        REQUIRE(dataModel.getUniformValue("myFloat").value()
            == ShaderUniformModel::GLUniformType(1.5f));
        REQUIRE(dataModel.getUniformValue("myInt").value()
            == ShaderUniformModel::GLUniformType(-15));
        REQUIRE(dataModel.getUniformValue("myUint").value()
            == ShaderUniformModel::GLUniformType(2));
    }
}

TEST_CASE("Data_ShaderUniformModel_getUniformValue") {
    auto dataModel = ShaderUniformModel();

    SECTION("Returns nullopts if not found") {
        REQUIRE_FALSE(dataModel.getUniformValue("missing").has_value());
    }
}
