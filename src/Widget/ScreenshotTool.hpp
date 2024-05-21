#pragma once

#include <memory>
#include <future>

#include "File/ImageFileCapture.hpp"
#include "Window/IPane.hpp"

#include <Basil/Packages/App.hpp>

namespace basil {

// TODO(sholloway): Documentation
// TODO(sholloway): Test coverage
// TODO(sholloway): (semi-related) De-register callbacks in all places
// TODO(sholloway): (semi-related) Try out using builder
// output in multiple parts of builder
// TODO(sholloway): Builder
class ScreenshotTool : public IBasilWidget,
                       public IBuildable<ScreenshotTool> {
 public:
    ScreenshotTool();

    ~ScreenshotTool();

    void onStart() override;

    void onLoop() override;

    void onStop() override;

    void requestCapture();

    void setTriggerKey(int keyCode) {
        this->triggerKey = keyCode;
    }

    void setSaveDirectory(std::filesystem::path savePath) {
        this->savePath = savePath;
    }

    std::filesystem::path getSaveDirectory() {
        return savePath;
    }

    void setSaveFileName(std::filesystem::path saveName) {
        this->saveName = saveName;
    }

    std::filesystem::path getFileName() {
        return saveName;
    }

    void setFocusPane(std::shared_ptr<IPane> pane) {
        this->focusPane = pane;
    }

 private:
    void readyState();
    void workingState();
    void onKeyPress(int keyCode, int scancode, int action, int mods);
    ImageCaptureArea areaFromPane(PaneProps paneProps);

    std::shared_ptr<IPane> focusPane = nullptr;
    ImageFileCapture fileCapture;
    std::future<bool> taskFuture;

    static inline const std::filesystem::path DEFAULT_SAVE_NAME
        = "image.png";
    std::filesystem::path savePath;
    std::filesystem::path saveName = DEFAULT_SAVE_NAME;

    int triggerKey = GLFW_KEY_UNKNOWN;

    enum class CaptureState {
        IDLE, READY, CAPTURING
    };
    CaptureState state = CaptureState::IDLE;

    unsigned int callbackID = -1;
    static inline unsigned int captureIndex = 0;

    Logger& logger = Logger::get();
};

}  // namespace basil
