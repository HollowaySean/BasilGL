#include "ScreenshotTool.hpp"

#include <fmt/format.h>

#include <thread>
#include <utility>

namespace basil {

ScreenshotTool::ScreenshotTool() : IBasilWidget({
    "ScreenshotTool",
    ProcessOrdinal::LATE,
    ProcessPrivilege::NONE,
    WidgetPubSubPrefs::NONE
}), fileCapture(ImageFileCapture()) {}

ScreenshotTool::~ScreenshotTool() {
    BasilContext::removeGLFWKeyCallback(callbackID);
}

void ScreenshotTool::onStart() {
    if (savePath == "") {
        savePath = std::filesystem::temp_directory_path();
        logger.log(
            fmt::format("No save path provided, using {}", savePath.c_str()),
            LogLevel::INFO);
    }

    using std::placeholders::_1, std::placeholders::_2,
          std::placeholders::_3, std::placeholders::_4;
    callbackID = BasilContext::setGLFWKeyCallback(
        std::bind(&ScreenshotTool::onKeyPress, this, _1, _2, _3, _4));
}

void ScreenshotTool::onLoop() {
    // TODO(sholloway): Move to functions
    switch (state) {
        case CaptureState::IDLE:
            return;

        case CaptureState::READY:
            taskFuture = fileCapture.captureAsync(getSaveFilePath());

            state = CaptureState::CAPTURING;
            return;
        case CaptureState::CAPTURING:
            auto status = taskFuture.wait_for(std::chrono::milliseconds(0));

            if (status == std::future_status::ready) {
                bool result = taskFuture.get();

                if (result) {
                    logger.log(
                        fmt::format("Capture saved to {}",
                            getSaveFilePath().c_str()),
                        LogLevel::INFO);
                } else {
                    logger.log(
                        "Unable to capture screenshot.",
                        LogLevel::WARN);
                }

                state = CaptureState::IDLE;
            }
    }
}

void ScreenshotTool::onStop() {
    BasilContext::removeGLFWKeyCallback(callbackID);
}

void ScreenshotTool::onKeyPress(
        int keyCode, int scancode, int action, int mods) {
    if (keyCode == triggerKey && action == GLFW_PRESS) {
        requestCapture();
    }
}

}  // namespace basil
