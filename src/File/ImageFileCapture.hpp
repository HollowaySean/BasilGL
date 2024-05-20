#pragma once

#include <filesystem>
#include <future>
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
// TODO(sholloway): Cleanup & logging
// TODO(sholloway): Resizing logic
class ImageFileCapture : private IBasilContextConsumer {
 public:
    ImageFileCapture();

    ~ImageFileCapture();

    bool capture(std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea = std::nullopt);

    std::future<bool> captureAsync(std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea = std::nullopt);

 private:
    void updateBufferSize(
        int width, int height);
    bool saveBufferToFile(
        GLubyte* dataPointer, ImageCaptureArea area,
        std::filesystem::path savePath);
    GLubyte* copyFrameToBuffer();

    GLint width = 0, height = 0;
    unsigned int pixelBufferID = -1;

    LOGGER_FORMAT LOG_CAPTURE_SUCCESS =
        "Successfully saved capture to file: {}";
    LOGGER_FORMAT LOG_CAPTURE_FAILURE =
        "Failed to save capture data to file.";

    static inline Logger& logger = Logger::get();
};

}  // namespace basil
