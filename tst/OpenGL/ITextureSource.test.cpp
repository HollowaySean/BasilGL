#include <catch.hpp>

#include "OpenGL/ITextureSource.hpp"

using basil::ITextureSource;

template<int N>
class TestTextureSource: public ITextureSource<N> {
 public:
    const void* data() override { return 0; }
};

TEST_CASE("OpenGL_ITextureSource_setDimensions") {
    auto source = TestTextureSource<3>();

    SECTION("Sets dimensions correctly with variadic input") {
        source.setWidth(1);
        source.setHeight(2);
        source.setDepth(3);

        CHECK(source.getWidth()  == 1);
        CHECK(source.getHeight() == 2);
        CHECK(source.getDepth()  == 3);
    }
}
