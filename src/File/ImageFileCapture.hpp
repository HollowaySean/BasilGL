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

/** @brief Utility class which captures portion or entirety of
 *  window, and saves to an image file. */
class ImageFileCapture : private IBasilContextConsumer {
 public:
    /** @brief Initialize pixel buffer and construct object. */
    ImageFileCapture();

    /** @brief Delete pixel buffer and deconstruct object. */
    ~ImageFileCapture();

    /** @brief Synchronously capture screen as image file. */
    bool capture(std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea = std::nullopt);

    /** @brief Asynchronously capture screen as image file.
     *  @note  Buffer unmapping must be done within context-owning
     *  thread. If captureAsync is used, call clearBuffer method
     *  after capture is complete. */
    std::future<bool> captureAsync(std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea = std::nullopt);

    /** @brief Unmaps pixel buffer object and binds default buffer. */
    void clearBuffer();

    /** @brief Set quality level for JPEG compression, from 0 to 100. */
    void setJPEGQuality(unsigned int quality) {
        jpegQuality = quality;
    }

    /** @brief Set compression level for PNG files. */
    void setPNGCompression(unsigned int compression) {
        pngCompression = compression;
    }

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

    unsigned int jpegQuality = 80;
    unsigned int pngCompression = 8;

    unsigned int callbackID;

    LOGGER_FORMAT LOG_BUFFER_CREATED =
        "Pixel Buffer (ID{:02}) - Pixel buffer created.";
    LOGGER_FORMAT LOG_BUFFER_DELETED =
        "Pixel Buffer (ID{:02}) - Pixel buffer deleted.";

    LOGGER_FORMAT LOG_EMPTY_EXTENSION =
        "No file extension found, saving as PNG file";
    LOGGER_FORMAT LOG_BAD_EXTENSION =
        "Unknown file extension: \"{}\"";

    LOGGER_FORMAT LOG_COPY_FAILURE =
        "Could not copy screen data to buffer.";
    LOGGER_FORMAT LOG_CAPTURE_SUCCESS =
        "Successfully saved capture to file: {}";
    LOGGER_FORMAT LOG_CAPTURE_FAILURE =
        "Failed to save capture data to file.";

    static inline Logger& logger = Logger::get();
};

}  // namespace basil
