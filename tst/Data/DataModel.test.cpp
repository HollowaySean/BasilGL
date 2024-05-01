#include <catch.hpp>

#include <Basil/Data.hpp>

using basil::DataModel;

TEST_CASE("Data_DataModel_setUniformValue") {
    auto dataModel = DataModel();
    dataModel.setUniformValue(true, "myBool");
    dataModel.setUniformValue(1.5f, "myFloat");
    dataModel.setUniformValue(-15, "myInt");
    dataModel.setUniformValue(2, "myUint");

    SECTION("Saves uniforms to container") {
        REQUIRE(dataModel.getUniforms().size() == 4);

        REQUIRE(dataModel.getUniformValue("myBool").value()
            == DataModel::GLUniformType(true));
        REQUIRE(dataModel.getUniformValue("myFloat").value()
            == DataModel::GLUniformType(1.5f));
        REQUIRE(dataModel.getUniformValue("myInt").value()
            == DataModel::GLUniformType(-15));
        REQUIRE(dataModel.getUniformValue("myUint").value()
            == DataModel::GLUniformType(2));
    }
}

TEST_CASE("Data_DataModel_getUniformValue") {
    auto dataModel = DataModel();

    SECTION("Returns nullopts if not found") {
        REQUIRE_FALSE(dataModel.getUniformValue("missing").has_value());
    }
}
