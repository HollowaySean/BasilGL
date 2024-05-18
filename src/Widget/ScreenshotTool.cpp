#include "ScreenshotTool.hpp"

#include <fmt/format.h>

namespace basil {

ScreenshotTool::ScreenshotTool() : IBasilWidget({
    "ScreenshotTool",
    ProcessOrdinal::LATE,
    ProcessPrivilege::NONE,
    WidgetPubSubPrefs::NONE
}) {}

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

    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    using std::placeholders::_4;
    callbackID = BasilContext::setGLFWKeyCallback(
        std::bind(&ScreenshotTool::onKeyPress, this, _1, _2, _3, _4));
}

void ScreenshotTool::onLoop() {
    if (!readyToCapture) return;
    readyToCapture = false;

    std::filesystem::path fullPath = savePath / saveName;
    bool result =
        ImageFileCapture::capture(fullPath);

    if (result) {
        logger.log(
            fmt::format("Capture saved to {}", fullPath.c_str()),
            LogLevel::INFO);
    } else {
        logger.log(
            "Unable to capture screenshot.",
            LogLevel::WARN);
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
