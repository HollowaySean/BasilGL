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
// TODO(sholloway): Branch on filetype
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
    GLubyte* copyFrameToBuffer(
        ImageCaptureArea area);

    GLint width = 0, height = 0;
    unsigned int pixelBufferID = -1;

    LOGGER_FORMAT LOG_BUFFER_CREATED =
        "Pixel Buffer (ID{:02}) - Pixel buffer created.";
    LOGGER_FORMAT LOG_BUFFER_DELETED =
        "Pixel Buffer (ID{:02}) - Pixel buffer deleted.";

    LOGGER_FORMAT LOG_COPY_FAILURE =
        "Could not copy screen data to buffer.";
    LOGGER_FORMAT LOG_CAPTURE_SUCCESS =
        "Successfully saved capture to file: {}";
    LOGGER_FORMAT LOG_CAPTURE_FAILURE =
        "Failed to save capture data to file.";

    static inline Logger& logger = Logger::get();
};

}  // namespace basil
