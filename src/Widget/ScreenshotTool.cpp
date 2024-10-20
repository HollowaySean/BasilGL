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
            fmt::format("No save path provided, using {}", savePath.string().c_str()),
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

void ScreenshotTool::onStop() {
    BasilContext::removeGLFWKeyCallback(callbackID);
}

void ScreenshotTool::readyState() {
    std::optional<ViewArea> paneArea = std::nullopt;
    if (focusPane) {
        paneArea = std::optional(focusPane->viewArea);
    }

    auto runtimeName = fmt::runtime(saveName.string().c_str());
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

void ScreenshotTool::requestCapture() {
    if (state < CaptureState::READY) {
        state = CaptureState::READY;
    }
}

void ScreenshotTool::onKeyPress(
        int keyCode, int scancode, int action, int mods) {
    if (keyCode == triggerKey && action == GLFW_PRESS) {
        requestCapture();
    }
}

ScreenshotTool::Builder&
ScreenshotTool::Builder::withTriggerKey(int keyCode) {
    this->impl->setTriggerKey(keyCode);
    return (*this);
}

ScreenshotTool::Builder&
ScreenshotTool::Builder::withSaveDirectory(std::filesystem::path savePath) {
    this->impl->setSaveDirectory(savePath);
    return (*this);
}

ScreenshotTool::Builder&
ScreenshotTool::Builder::withSaveFileName(std::filesystem::path saveName) {
    this->impl->setSaveFileName(saveName);
    return (*this);
}

ScreenshotTool::Builder&
ScreenshotTool::Builder::withFocusPane(std::shared_ptr<IPane> pane) {
    this->impl->setFocusPane(pane);
    return (*this);
}

}  // namespace basil
