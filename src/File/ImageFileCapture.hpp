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
class ImageFileCapture {
 public:
    static bool capture(std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea = std::nullopt);

 private:
    static ImageCaptureArea getWindowCaptureArea();

    LOGGER_FORMAT LOG_CAPTURE_SUCCESS =
        "Successfully saved capture to file: {}";
    LOGGER_FORMAT LOG_CAPTURE_FAILURE =
        "Failed to save capture data to file.";

    static inline Logger& logger = Logger::get();
};

}  // namespace basil
