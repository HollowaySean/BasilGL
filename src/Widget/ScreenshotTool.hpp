#pragma once

#include <memory>
#include <future>

#include "File/ImageFileCapture.hpp"
#include "Window/IPane.hpp"

#include <Basil/Packages/App.hpp>

namespace basil {

// TODO(sholloway): Documentation
// TODO(sholloway): Test coverage
// TODO(sholloway): Cleanup logging
// TODO(sholloway): Filename support
class ScreenshotTool : public IBasilWidget,
                       public IBuildable<ScreenshotTool> {
 public:
    ScreenshotTool();

    ~ScreenshotTool();

    void onStart() override;

    void onLoop() override;

    void onStop() override;

    void requestCapture() {
        if (state < CaptureState::READY) {
            state = CaptureState::READY;
        }
    }

    void setTriggerKey(int keyCode) {
        this->triggerKey = keyCode;
    }

    void setSaveDirectory(std::filesystem::path savePath) {
        this->savePath = savePath;
    }

    std::filesystem::path getSaveFilePath() {
        return savePath / saveName;
    }

    void watchPane(std::shared_ptr<IPane> pane) {
        this->paneToWatch = pane;
    }

 private:
    std::shared_ptr<IPane> paneToWatch = nullptr;
    ImageCaptureArea areaFromPane(PaneProps paneProps);

    void onKeyPress(int keyCode, int scancode, int action, int mods);

    ImageFileCapture fileCapture;

    std::filesystem::path savePath;
    std::filesystem::path saveName = "image.png";

    int triggerKey = GLFW_KEY_UNKNOWN;

    enum class CaptureState {
        IDLE, READY, CAPTURING
    };
    CaptureState state = CaptureState::IDLE;

    std::future<bool> taskFuture;

    unsigned int callbackID = -1;

    Logger& logger = Logger::get();
};

}  // namespace basil
