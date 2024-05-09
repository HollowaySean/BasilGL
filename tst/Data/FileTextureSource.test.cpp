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

        CHECK(source.getWidth() == 256);
        CHECK(source.getHeight() == 256);

        CHECK(source.format.format == GL_RGBA);
        CHECK(source.format.internalFormat == GL_RGBA8);
        CHECK(source.format.type == GL_UNSIGNED_BYTE);

        CHECK(source.data() == source.imageData);
    }

    SECTION("Logs error if file does not exist") {
        auto badPath = "fdjsklafjdklas";
        auto source = FileTextureSource(badPath);

        Logger& logger = Logger::get();
        CHECK(logger.getLastLevel() == LogLevel::ERROR);

        CHECK(source.getWidth() == 0);
        CHECK(source.getHeight() == 0);
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

        CHECK(source->getWidth() == 256);
        CHECK(source->getHeight() == 256);

        CHECK(source->format.format == GL_RGBA);
        CHECK(source->format.internalFormat == GL_RGBA8);
        CHECK(source->format.type == GL_UNSIGNED_BYTE);

        CHECK(source->data() == source->imageData);
    }
}
