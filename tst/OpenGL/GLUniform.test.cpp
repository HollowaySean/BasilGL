#include <vector>

#include <catch.hpp>

#include "OpenGL/GLUniform.hpp"

using basil::GLUniformVector;

TEST_CASE("OpenGL_GLUniform_GLUniformVector") {
    std::vector<float> data = { 0.f, 1.f, 2.f, 3.f };
    std::vector<bool> bData = { true, false, true, false };

    SECTION("Uses vector length if length not provided") {
        auto uniform = GLUniformVector<float>(data, "name");

        CHECK(uniform.getLength() == 4);
        CHECK(uniform.getCount() == 1);
    }

    SECTION("Uses vector length if length not provided (bool specialization)") {
        auto uniform = GLUniformVector<bool>(bData, "name");

        CHECK(uniform.getLength() == 4);
        CHECK(uniform.getCount() == 1);
    }

    SECTION("Uses vector size if count not provided") {
        auto uniform = GLUniformVector<float>(data, "name", 2, 1);

        CHECK(uniform.getLength() == 2);
        CHECK(uniform.getWidth() == 1);
        CHECK(uniform.getCount() == 2);
    }

    SECTION("Uses vector size if count not provided (bool specialization)") {
        auto uniform = GLUniformVector<bool>(bData, "name", 2, 1);

        CHECK(uniform.getLength() == 2);
        CHECK(uniform.getWidth() == 1);
        CHECK(uniform.getCount() == 2);
    }
}
