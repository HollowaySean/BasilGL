#pragma once

#include "File/ImageFileCapture.hpp"

#include <Basil/Packages/App.hpp>
#include <Basil/Packages/Context.hpp>

namespace basil {

// TODO(sholloway): Documentation
// TODO(sholloway): Test coverage
// TODO(sholloway): Way to watch pane? Other options?
class ScreenshotTool : public IBasilWidget,
                       public IBuildable<ScreenshotTool>,
                       private IBasilContextConsumer {
 public:
    ScreenshotTool();

    ~ScreenshotTool();

    void onStart() override;

    void onLoop() override;

    void onStop() override;

    void requestCapture() {
        this->readyToCapture = true;
    }

    void setTriggerKey(int keyCode) {
        this->triggerKey = keyCode;
    }

    void setSaveDirectory(std::filesystem::path savePath) {
        this->savePath = savePath;
    }

 private:
    void onKeyPress(int keyCode, int scancode, int action, int mods);

    std::filesystem::path savePath;
    std::filesystem::path saveName = "image.png";

    int triggerKey = GLFW_KEY_UNKNOWN;
    bool readyToCapture = false;
    unsigned int callbackID = -1;

    Logger& logger = Logger::get();
};

}  // namespace basil
