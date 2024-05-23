#include <catch.hpp>

#include "File/ImageFileCapture.hpp"

#include "File/FileTestUtils.hpp"
#include "OpenGL/GLTestUtils.hpp"

using basil::BasilContext;
using basil::ViewArea;
using basil::ImageFileCapture;

TEST_CASE("File_ImageFileCapture_ImageFileCapture") { BASIL_LOCK_TEST
    SECTION("Initializes pixel buffer.") {
        auto capture = ImageFileCapture();
        CHECK(capture.pixelBufferID != -1);
    }

    SECTION("Sets up frame height and width callbacks.") {
        auto window = BasilContext::getGLFWWindow();
        glfwSetWindowSize(window,
            BASIL_DEFAULT_WINDOW_WIDTH_TEST, BASIL_DEFAULT_WINDOW_HEIGHT_TEST);

        // GLFW window size requires a slight delay
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        auto capture = ImageFileCapture();

        CHECK(capture.width == BASIL_DEFAULT_WINDOW_WIDTH_TEST);
        CHECK(capture.height == BASIL_DEFAULT_WINDOW_HEIGHT_TEST);

        CHECK(BasilContext::framebufferCallbacks.contains(capture.callbackID));
    }
}

TEST_CASE("File_ImageFileCapture_copyFrameToBuffer") { BASIL_LOCK_TEST
    auto capture = ImageFileCapture();

    SECTION("Returns pointer to PBO memory access") {
        auto result = capture.copyFrameToBuffer({ 1, 1, 0, 0 });
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

TEST_CASE("File_ImageFileCapture_capture") { BASIL_LOCK_TEST
    ViewArea area = { 1, 1, 0, 0 };
    auto capture = ImageFileCapture();

    SECTION("Returns true on successful capture") {
        auto path = FileTestUtils::setUpTempDir("capture-success.png");

        bool result = capture.capture(path, area);

        CHECK(result);
        CHECK(std::filesystem::exists(path));
    }

    SECTION("Returns false on unsuccessful copy") {
        auto path = FileTestUtils::setUpTempDir("capture-failure.png");

        BasilContext::terminate();
        bool result = capture.capture(path, area);

        CHECK_FALSE(result);
        CHECK_FALSE(std::filesystem::exists(path));
    }
}

TEST_CASE("File_ImageFileCapture_captureAsync") { BASIL_LOCK_TEST
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

    SECTION("Returns false on unsuccessful copy") {
        auto path = FileTestUtils::setUpTempDir("capture-async-failure.png");

        BasilContext::terminate();
        std::future<bool> future = capture.captureAsync(path, area);

        future.wait();
        bool result = future.get();

        CHECK_FALSE(result);
        CHECK_FALSE(std::filesystem::exists(path));
    }
}
