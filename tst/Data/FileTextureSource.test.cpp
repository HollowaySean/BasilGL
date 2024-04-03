#include <catch.hpp>

#include "Data/FileTextureSource.hpp"

using basil::FileTextureSource;
using basil::Logger;
using basil::LogLevel;

TEST_CASE("Data_FileTextureSource_FileTextureSource") {
    auto testImagePath =
        std::filesystem::path(TEST_DIR) / "Data/assets/test-img.jpg";

    SECTION("Loads image file") {
        auto source = FileTextureSource(testImagePath);

        REQUIRE(source.getWidth() == 256);
        REQUIRE(source.getHeight() == 256);

        REQUIRE(source.format.format == GL_RGBA);
        REQUIRE(source.format.internalFormat == GL_RGBA);
        REQUIRE(source.format.type == GL_UNSIGNED_BYTE);

        REQUIRE(source.data() == source.imageData);
    }

    SECTION("Logs error if file does not exist") {
        auto badPath = "fdjsklafjdklas";
        auto source = FileTextureSource(badPath);

        Logger& logger = Logger::get();
        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);

        REQUIRE(source.getWidth() == 0);
        REQUIRE(source.getHeight() == 0);
    }
}

TEST_CASE("Data_FileTextureSource_Builder") {
    auto testImagePath =
        std::filesystem::path(TEST_DIR) / "Data/assets/test-img.jpg";

    SECTION("Builds from file path") {
        auto source =
            FileTextureSource::Builder()
                .fromFilePath(testImagePath)
                .build();

        REQUIRE(source->getWidth() == 256);
        REQUIRE(source->getHeight() == 256);

        REQUIRE(source->format.format == GL_RGBA);
        REQUIRE(source->format.internalFormat == GL_RGBA);
        REQUIRE(source->format.type == GL_UNSIGNED_BYTE);

        REQUIRE(source->data() == source->imageData);
    }
}
