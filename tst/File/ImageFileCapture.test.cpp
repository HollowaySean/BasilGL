#include <catch.hpp>

#include "File/ImageFileCapture.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::BasilContext;
using basil::ImageCaptureArea;
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
