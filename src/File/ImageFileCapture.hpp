#pragma once

#include <filesystem>
#include <optional>
#include <vector>

#include <Basil/Packages/Context.hpp>
#include <Basil/Packages/Logging.hpp>

namespace basil {

struct ImageCaptureArea {
    int width, height, xOffset, yOffset;
};

// TODO(sholloway): Test coverage
// TODO(sholloway): Documentation
// TODO(sholloway): Better handling of framebuffer
// TODO(sholloway): Make non-static
class ImageFileCapture {
 public:
    bool capture(std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea = std::nullopt);

    ImageFileCapture() {
        initializeFramebuffer();
    }

 private:
    ImageCaptureArea getWindowCaptureArea();

    void initializeFramebuffer();

    unsigned int framebufferID = -1;
    unsigned int renderbufferID = -1;
    unsigned int textureID = -1;

    LOGGER_FORMAT LOG_CAPTURE_SUCCESS =
        "Successfully saved capture to file: {}";
    LOGGER_FORMAT LOG_CAPTURE_FAILURE =
        "Failed to save capture data to file.";

    static inline Logger& logger = Logger::get();
};

}  // namespace basil
