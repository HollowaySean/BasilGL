#include <catch.hpp>

#include "Widget/ScreenshotTool.hpp"

#include "File/FileTestUtils.hpp"
#include "OpenGL/GLTestUtils.hpp"
#include "Window/WindowTestUtils.hpp"

using basil::BasilContext;
using basil::IPane;
using basil::ScreenshotTool;

TEST_CASE("Widget_ScreenshotTool_onStart") { BASIL_LOCK_TEST
    auto widget = ScreenshotTool();
    widget.onStart();

    SECTION("Uses temp directory if no save path provided") {
        CHECK(widget.getSaveDirectory() != "");
    }

    SECTION("Sets key press callback") {
        CHECK(BasilContext::get().keyCallbacks.contains(widget.callbackID));
    }
}

TEST_CASE("Widget_ScreenshotTool_onLoop") { BASIL_LOCK_TEST
    auto widget = ScreenshotTool();

    SECTION("Does nothing if in idle state") {
        widget.state = ScreenshotTool::CaptureState::IDLE;
        widget.onLoop();
        CHECK(widget.state == ScreenshotTool::CaptureState::IDLE);
    }

    SECTION("Dispatches screen capture and returns to idle if ready") {
        auto path = FileTestUtils::setUpTempDir("ScreenshotTool_onLoop_1");
        std::shared_ptr<IPane> pane = std::make_shared<TestPane>(ViewArea());

        widget.setSaveDirectory(path.parent_path());
        widget.setSaveFileName(path.stem());
        widget.setFocusPane(pane);

        widget.requestCapture();
        widget.onLoop();

        CHECK(widget.state == ScreenshotTool::CaptureState::CAPTURING);

        widget.taskFuture.wait();
        CHECK(std::filesystem::exists(path));

        widget.onLoop();
        CHECK(widget.state == ScreenshotTool::CaptureState::IDLE);
    }
}

TEST_CASE("Widget_ScreenshotTool_onStop") { BASIL_LOCK_TEST
    auto widget = ScreenshotTool();
    widget.onStart();

    SECTION("Deregisters callbacks") {
        CHECK(BasilContext::get()
            .keyCallbacks.contains(widget.callbackID));
        widget.onStop();
        CHECK_FALSE(BasilContext::get()
            .keyCallbacks.contains(widget.callbackID));
    }
}

TEST_CASE("Widget_ScreenshotTool_onKeyPress") {
    auto widget = ScreenshotTool();
    widget.setTriggerKey(GLFW_KEY_0);

    SECTION("Sets capture ready if key matches and is pressed") {
        widget.onKeyPress(GLFW_KEY_0, 0, GLFW_PRESS, 0);
        CHECK(widget.state == ScreenshotTool::CaptureState::READY);
    }

    SECTION("Does nothing if key does not match") {
        widget.onKeyPress(GLFW_KEY_1, 0, GLFW_PRESS, 0);
        CHECK(widget.state == ScreenshotTool::CaptureState::IDLE);
    }

    SECTION("Does nothing if state is not press") {
        widget.onKeyPress(GLFW_KEY_0, 0, GLFW_RELEASE, 0);
        CHECK(widget.state == ScreenshotTool::CaptureState::IDLE);
    }
}

TEST_CASE("Widget_ScreenshotTool_Builder") {
    std::shared_ptr<IPane> pane = std::make_shared<TestPane>(ViewArea());

    auto widget = ScreenshotTool::Builder()
        .withTriggerKey(GLFW_KEY_9)
        .withSaveDirectory("directory")
        .withSaveFileName("file")
        .withFocusPane(pane)
        .build();

    SECTION("Builds correctly") {
        CHECK(widget->triggerKey == GLFW_KEY_9);
        CHECK(widget->getSaveDirectory() == "directory");
        CHECK(widget->getFileName() == "file");
        CHECK(widget->focusPane == pane);
    }
}
