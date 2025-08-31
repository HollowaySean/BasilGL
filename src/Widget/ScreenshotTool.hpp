#pragma once

#include <memory>
#include <future>

#include "File/ImageFileCapture.hpp"
#include "Window/IPane.hpp"

#include <Basil/Packages/App.hpp>

namespace basil {

/** @brief Basil widget to asynchronously take screenshots on key press. */
class ScreenshotTool : public IBasilWidget,
                       public IBuildable<ScreenshotTool>,
                       protected IBasilContextConsumer {
 public:
    /** @brief Initialize ScreenshotTool. */
    ScreenshotTool();

    /** @brief Deinitialize ScreenshotTool.*/
    ~ScreenshotTool();

    /** @brief Set file path and callbacks. */
    void onStart() override;

    /** @brief Dispatch async image capture, and check for
     *  completion of dispatched threads. */
    void onLoop() override;

    /** @brief Remove key callback on stop. */
    void onStop() override;

    /** @brief Initiate screen capture process. */
    void requestCapture();

    /** @brief Set key code to trigger on, i.e. GLFW_KEY_S */
    void setTriggerKey(int keyCode) {
        this->triggerKey = keyCode;
    }

    /** @brief Set directory to save screenshots to.
     *  @note If empty, a temp folder will be created and used. */
    void setSaveDirectory(std::filesystem::path savePath) {
        this->savePath = savePath;
    }

    /** @returns Path of file save directory. */
    std::filesystem::path getSaveDirectory() {
        return savePath;
    }

    /** @brief Set name of file to save to. File type is deduced from
     *  the file extension. FMT format string can be used with arguments
     *  {index}, which provides an increasing counter, and {time}, which
     *  provides the current timestamp. */
    void setSaveFileName(std::filesystem::path saveName) {
        this->saveName = saveName;
    }

    /** @returns Name of file to save to. */
    std::filesystem::path getFileName() {
        return saveName;
    }

    /** @brief Set IPane pointer to focus screenshot on.
     *  If null, full window is used by default. */
    void setFocusPane(std::shared_ptr<IPane> pane) {
        this->focusPane = pane;
    }

    /** @brief Builder pattern for ScreenshotTool. */
    class Builder : public IBuilder<ScreenshotTool> {
     public:
        /** @brief Set trigger key code. */
        Builder& withTriggerKey(int keyCode);

        /** @brief Set directory to save screenshots to. */
        Builder& withSaveDirectory(std::filesystem::path savePath);

        /** @brief Set name of file save to. */
        Builder& withSaveFileName(std::filesystem::path saveName);

        /** @brief Set IPane to focus screenshot on.*/
        Builder& withFocusPane(std::shared_ptr<IPane> pane);
    };

#ifndef TEST_BUILD

 private:
#endif
    void readyState();
    void workingState();
    void onKeyPress(int keyCode, int scancode, int action, int mods);

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
