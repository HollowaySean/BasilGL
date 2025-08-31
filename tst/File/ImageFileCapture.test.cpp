#include <catch.hpp>

#include "File/ImageFileCapture.hpp"

#include "File/FileTestUtils.hpp"
#include "OpenGL/GLTestUtils.hpp"

using basil::BasilContext;
using basil::ViewArea;
using basil::ImageFileCapture;

TEST_CASE("File_ImageFileCapture_ImageFileCapture") {
    SECTION("Initializes pixel buffer.") {
        auto capture = ImageFileCapture();
        CHECK(capture.pixelBufferID != (unsigned int)-1);
    }
}

TEST_CASE("File_ImageFileCapture_copyFrameToBuffer") {
    auto capture = ImageFileCapture();
    capture.updateBufferSize(1, 1);

    SECTION("Returns pointer to PBO memory access") {
        auto result = capture.copyFrameToBuffer({1, 1, 0, 0});
        CHECK(result != nullptr);
    }
}

TEST_CASE("File_ImageFileCapture_saveBufferToFile") {
    std::vector<unsigned char> data
        = std::vector<unsigned char>({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 });
    GLubyte* dataPointer = static_cast<GLubyte*>(data.data());

    ViewArea area = { 2, 2, 0, 0 };
    auto capture = ImageFileCapture();

    SECTION("Saves JPG to file") {
        auto path = FileTestUtils::setUpTempDir("file.jpg");
        bool result = capture.saveBufferToFile(dataPointer, area, path);

        CHECK(result);
        CHECK(std::filesystem::exists(path));
    }

    SECTION("Saves BMP to file") {
        auto path = FileTestUtils::setUpTempDir("file.bmp");
        bool result = capture.saveBufferToFile(dataPointer, area, path);

        CHECK(result);
        CHECK(std::filesystem::exists(path));
    }

    SECTION("Saves PNG to file") {
        auto path = FileTestUtils::setUpTempDir("file.png");
        bool result = capture.saveBufferToFile(dataPointer, area, path);

        CHECK(result);
        CHECK(std::filesystem::exists(path));
    }

    SECTION("Defaults to PNG") {
        auto path = FileTestUtils::setUpTempDir("file");
        bool result = capture.saveBufferToFile(dataPointer, area, path);

        CHECK(result);
        CHECK(std::filesystem::exists(path));
    }

    SECTION("Does not save for unknown extension") {
        auto path = FileTestUtils::setUpTempDir("file.xyz");
        bool result = capture.saveBufferToFile(dataPointer, area, path);

        CHECK_FALSE(result);
        CHECK_FALSE(std::filesystem::exists(path));
    }
}

TEST_CASE("File_ImageFileCapture_capture") {
    ViewArea area = { 1, 1, 0, 0 };
    auto capture = ImageFileCapture();

    SECTION("Returns true on successful capture") {
        auto path = FileTestUtils::setUpTempDir("capture-success.png");

        bool result = capture.capture(path, area);

        CHECK(result);
        CHECK(std::filesystem::exists(path));
    }

    // TODO(sholloway): Find better way to test this

    // SECTION("Returns false on unsuccessful copy") {
    //     auto path = FileTestUtils::setUpTempDir("capture-failure.png");
    //     capture.updateBufferSize(0, 0);

    //     // Dereference GLFW window to force error
    //     BasilContext::get().glfwWindow = nullptr;
    //     bool result = capture.capture(path, area);

    //     CHECK_FALSE(result);
    //     CHECK_FALSE(std::filesystem::exists(path));
    // }
}

TEST_CASE("File_ImageFileCapture_captureAsync") {
    ViewArea area = { 1, 1, 0, 0 };
    auto capture = ImageFileCapture();

    SECTION("Returns true on successful capture") {
        auto path = FileTestUtils::setUpTempDir("capture-async-success.png");
        std::future<bool> future = capture.captureAsync(path, area);

        future.wait();
        bool result = future.get();

        CHECK(result);
        CHECK(std::filesystem::exists(path));
    }

    // SECTION("Returns false on unsuccessful copy") {
    //     auto path = FileTestUtils::setUpTempDir("capture-async-failure.png");
    //     capture.updateBufferSize(0, 0);

    //     // Dereference GLFW window to force error
    //     BasilContext::get().glfwWindow = nullptr;
    //     std::future<bool> future = capture.captureAsync(path, area);

    //     future.wait();
    //     bool result = future.get();

    //     CHECK_FALSE(result);
    //     CHECK_FALSE(std::filesystem::exists(path));
    // }
}
