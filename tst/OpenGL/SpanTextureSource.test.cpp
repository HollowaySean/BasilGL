#include <span>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <catch.hpp>

#include "OpenGL/SpanTextureSource.hpp"

using basil::SpanTextureSource;

TEST_CASE("OpenGL_SpanTextureSource_SpanTextureSource") {
    std::vector<float> data = { 1., 2., 3., 4. };
    auto source = SpanTextureSource<float, 1, 4>();
    source.setSource(data);

    SECTION("Points to contained span") {
        for (int i = 0; i < data.size(); i++) {
            auto expected = data.data()[i];
            auto actual = (reinterpret_cast<const float*>(source.data()))[i];

            CHECK(actual == expected);
        }
    }

    SECTION("Contains correct GLTextureFormat") {
        CHECK(source.format.type == GL_FLOAT);
        CHECK(source.format.format == GL_RGBA);
        CHECK(source.format.internalFormat == GL_RGBA32F);
    }
}

TEST_CASE("OpenGL_SpanTextureSource_Builder") {
    SECTION("Builds from span") {
        std::vector<float> data = { 1., 2., 3., 4. };
        auto source = SpanTextureSource<float>(data);

        CHECK(data.data() == source.data());
    }
}
