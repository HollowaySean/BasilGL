#include <catch.hpp>

#include "Data/ShaderUniformModel.hpp"

#include "OpenGL/GLUniform.hpp"
#include "OpenGL/GLTexture.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::GLUniformScalar;
using basil::IGLTexture;
using basil::GLTexture2D;
using basil::ShaderUniformModel;

TEST_CASE("Data_ShaderUniformModel_addUniform") {
    auto dataModel = ShaderUniformModel();
    // From GLUniform object
    unsigned int boolID = dataModel.addUniform(
        std::make_shared<GLUniformScalar<bool>>(true, "myBool"));
    // From scalar value
    unsigned int floatID = dataModel.addUniform(
        1.5f, "myFloat");
    // From pointer
    int intValue = -15;
    unsigned int intID = dataModel.addUniform(
        &intValue, "myInt");
    // From std::vector
    unsigned int uintID = dataModel.addUniform(
        std::vector<unsigned int>({ 2, 4 }), "myUint");

    // Overwrite case
    dataModel.addUniform(2.5f, "myFloat");

    SECTION("Saves uniforms to container by name") {
        CHECK(dataModel.getUniforms().size() == 4);

        CHECK(*(reinterpret_cast<bool*>(
            dataModel.getUniform("myBool").value()->getData()))
            == true);
        CHECK(*(reinterpret_cast<float*>(
            dataModel.getUniform("myFloat").value()->getData()))
            == 2.5f);
        CHECK(*(reinterpret_cast<int*>(
            dataModel.getUniform("myInt").value()->getData()))
            == intValue);
        CHECK((reinterpret_cast<unsigned int*>(
            dataModel.getUniform("myUint").value()->getData()))[0]
            == 2);
        CHECK((reinterpret_cast<unsigned int*>(
            dataModel.getUniform("myUint").value()->getData()))[1]
            == 4);
    }

    SECTION("Saves uniforms to container with ID") {
        CHECK(*(reinterpret_cast<bool*>(
            dataModel.getUniform(boolID).value()->getData()))
            == true);
        CHECK(*(reinterpret_cast<float*>(
            dataModel.getUniform(floatID).value()->getData()))
            == 2.5f);
        CHECK(*(reinterpret_cast<int*>(
            dataModel.getUniform(intID).value()->getData()))
            == intValue);
        CHECK((reinterpret_cast<unsigned int*>(
            dataModel.getUniform(uintID).value()->getData()))[0]
            == 2);
        CHECK((reinterpret_cast<unsigned int*>(
            dataModel.getUniform(uintID).value()->getData()))[1]
            == 4);
    }

    SECTION("Saves uniform containing texture location") {
        auto texture = std::make_shared<GLTexture2D>();

        unsigned int texID = dataModel.addUniform(texture, "myTex");

        CHECK(*(reinterpret_cast<int*>(
            dataModel.getUniform(texID).value()->getData()))
            == texture->getUniformLocation());
    }
}

TEST_CASE("Data_ShaderUniformModel_getUniform") {
    auto dataModel = ShaderUniformModel();
    unsigned int floatID = dataModel.addUniform(1.5f, "myFloat");

    SECTION("Get uniform by name, if it exists") {
        CHECK(*(reinterpret_cast<float*>(
            dataModel.getUniform("myFloat").value()->getData()))
            == 1.5f);
        CHECK_FALSE(dataModel.getUniform("myFloap").has_value());
    }

    SECTION("Get uniform by ID, if it exists") {
        CHECK(*(reinterpret_cast<float*>(
            dataModel.getUniform(floatID).value()->getData()))
            == 1.5f);
        CHECK_FALSE(dataModel.getUniform(-1).has_value());
    }
}

TEST_CASE("Data_ShaderUniformModel_setUniformValue") {
    auto dataModel = ShaderUniformModel();
    unsigned int ID = dataModel.addUniform(1.5f, "myFloat");

    SECTION("Set to GLUniform object") {
        CHECK(dataModel.setUniform(
            std::make_shared<GLUniformScalar<int>>(5, "myInt"), ID));
        CHECK_FALSE(dataModel.setUniform(
            std::make_shared<GLUniformScalar<int>>(5, "myInt"), -1));

        CHECK(*(reinterpret_cast<int*>(
            dataModel.getUniform(ID).value()->getData()))
            == 5);
    }

    SECTION("Set to pointer") {
        int uniformValue = 5;
        CHECK(dataModel.setUniformValue(&uniformValue, ID));
        CHECK_FALSE(dataModel.setUniformValue(&uniformValue, -1));

        CHECK(dataModel.getUniform(ID).value()->getData() == &uniformValue);
    }

    SECTION("Set to scalar value") {
        CHECK(dataModel.setUniformValue(5, ID));
        CHECK_FALSE(dataModel.setUniformValue(5, -1));

        CHECK(*(reinterpret_cast<int*>(
            dataModel.getUniform(ID).value()->getData()))
            == 5);
    }

    SECTION("Set to vector") {
        CHECK(dataModel.setUniformValue(std::vector<int>({5, 7}), ID));
        CHECK_FALSE(dataModel.setUniformValue(std::vector<int>({5, 7}), -1));

        CHECK((reinterpret_cast<int*>(
            dataModel.getUniform(ID).value()->getData()))[0]
            == 5);
        CHECK((reinterpret_cast<int*>(
            dataModel.getUniform(ID).value()->getData()))[1]
            == 7);
    }

    SECTION("Set to texture") {
        auto texture = std::make_shared<GLTexture2D>();
        CHECK(dataModel.setUniformValue(texture, ID));
        CHECK_FALSE(dataModel.setUniformValue(texture, -1));

        CHECK(*(reinterpret_cast<int*>(
            dataModel.getUniform(ID).value()->getData()))
            == texture->getUniformLocation());
    }
}

TEST_CASE("Data_ShaderUniformModel_Builder") {
    SECTION("Builds model correctly") {
        int myInt = -15;
        auto texture = std::make_shared<GLTexture2D>();

        auto model = ShaderUniformModel::Builder()
            .withUniform(
                std::make_shared<GLUniformScalar<float>>(1.5f, "myFloat"))
            .withUniform(5, "myUint")
            .withUniform(&myInt, "myInt")
            .withUniform(texture, "myTex")
            .build();

        CHECK(model->getUniforms().size() == 4);
    }
}
