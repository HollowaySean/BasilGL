#include "ScreenshotTool.hpp"

#include <fmt/chrono.h>

#include <string>
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
    switch (state) {
        case CaptureState::IDLE:
            return;

        case CaptureState::READY:
            readyState();
            return;

        case CaptureState::CAPTURING:
            workingState();
            return;
    }
}

void ScreenshotTool::readyState() {
    std::optional<ImageCaptureArea> paneArea = std::nullopt;
    if (focusPane) {
        paneArea = std::optional(
            areaFromPane(focusPane->paneProps));
    }

    auto runtimeName = fmt::runtime(saveName.c_str());
    auto timeStamp = std::chrono::round<std::chrono::seconds>(
        std::chrono::system_clock::now());
    std::filesystem::path formattedName = fmt::format(runtimeName,
        fmt::arg("index", captureIndex++),
        fmt::arg("time", timeStamp));

    auto formattedPath = savePath / formattedName;

    taskFuture = fileCapture.captureAsync(
        formattedPath, paneArea);

    state = CaptureState::CAPTURING;
}

void ScreenshotTool::workingState() {
    auto status = taskFuture.wait_for(
        FrameClock::duration::zero());

    if (status == std::future_status::ready) {
        state = CaptureState::IDLE;
        fileCapture.clearBuffer();
    }
}

void ScreenshotTool::onStop() {
    BasilContext::removeGLFWKeyCallback(callbackID);
}

void ScreenshotTool::requestCapture() {
    if (state < CaptureState::READY) {
        state = CaptureState::READY;
    }
}

ImageCaptureArea ScreenshotTool::areaFromPane(PaneProps paneProps) {
    return ImageCaptureArea {
        paneProps.width, paneProps.height,
        paneProps.xOffset, paneProps.yOffset
    };
}

void ScreenshotTool::onKeyPress(
        int keyCode, int scancode, int action, int mods) {
    if (keyCode == triggerKey && action == GLFW_PRESS) {
        requestCapture();
    }
}

}  // namespace basil
