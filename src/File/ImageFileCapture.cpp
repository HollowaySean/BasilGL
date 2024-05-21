#include "ImageFileCapture.hpp"

#include <fmt/format.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <string>

namespace basil {

ImageFileCapture::ImageFileCapture() {
    glGenBuffers(1, &pixelBufferID);
    logger.log(
        fmt::format(LOG_BUFFER_CREATED, pixelBufferID),
        LogLevel::DEBUG);

    GLFWwindow* window = BasilContext::getGLFWWindow();
    glfwGetFramebufferSize(window, &width, &height);
    updateBufferSize(width, height);

    using std::placeholders::_1, std::placeholders::_2;
    callbackID = BasilContext::setGLFWFramebufferSizeCallback(
        std::bind(&ImageFileCapture::updateBufferSize, this, _1, _2));
}

ImageFileCapture::~ImageFileCapture() {
    glDeleteBuffers(1, &pixelBufferID);
    logger.log(
        fmt::format(LOG_BUFFER_DELETED, pixelBufferID),
        LogLevel::DEBUG);

    BasilContext::removeGLFWFramebufferSizeCallback(callbackID);
}

void ImageFileCapture::updateBufferSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    int bytes = 3 * width * height;

    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
    glBufferData(GL_PIXEL_PACK_BUFFER, bytes, NULL, GL_STREAM_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

void ImageFileCapture::clearBuffer() {
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

bool ImageFileCapture::capture(
        std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea ) {
    auto area = captureArea.value_or(
        ImageCaptureArea { width, height, 0, 0 });

    auto pixelDataPointer = copyFrameToBuffer(area);

    if (!pixelDataPointer) {
        logger.log(
            fmt::format(LOG_COPY_FAILURE),
            LogLevel::ERROR);
        return false;
    }

    bool success = saveBufferToFile(pixelDataPointer, area, savePath);

    clearBuffer();
    return success;
}

std::future<bool> ImageFileCapture::captureAsync(
        std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea) {
    auto area = captureArea.value_or(
        ImageCaptureArea { width, height, 0, 0 });

    auto pixelDataPointer = copyFrameToBuffer(area);

    if (!pixelDataPointer) {
        logger.log(
            fmt::format(LOG_COPY_FAILURE),
            LogLevel::ERROR);
        auto failedPromise = std::promise<bool>();
        failedPromise.set_value(false);

        return failedPromise.get_future();
    }

    auto saveProcess = [this, pixelDataPointer, area, savePath] {
        return saveBufferToFile(pixelDataPointer, area, savePath);
    };
    auto future = std::async(std::launch::async, saveProcess);
    return future;
}

GLubyte* ImageFileCapture::copyFrameToBuffer(ImageCaptureArea area) {
    glReadBuffer(GL_FRONT);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
    glReadPixels(
        area.xOffset, area.yOffset, area.width, area.height,
        GL_RGB, GL_UNSIGNED_BYTE, 0);

    GLubyte* pixelDataPointer = static_cast<GLubyte*>(
        glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
    return pixelDataPointer;
}

bool ImageFileCapture::saveBufferToFile(
        GLubyte* dataPointer,
        ImageCaptureArea area,
        std::filesystem::path savePath ) {
    GLsizei channels = 3;
    GLsizei stride = channels * area.width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * area.height;

    // Write to file
    stbi_flip_vertically_on_write(true);
    const std::string extension = savePath.extension();

    int result;
    if (extension == ".jpg") {
        result = stbi_write_jpg(savePath.c_str(), area.width, area.height,
            channels, dataPointer, jpegQuality);

    } else if (extension == ".bmp") {
        result = stbi_write_bmp(savePath.c_str(), area.width, area.height,
            channels, dataPointer);

    } else if (extension == ".png") {
        stbi_write_png_compression_level = pngCompression;
        result = stbi_write_png(savePath.c_str(), area.width, area.height,
            channels, dataPointer, stride);

    } else if (extension.empty()) {
        logger.log(
            fmt::format(LOG_EMPTY_EXTENSION),
            LogLevel::INFO);

        stbi_write_png_compression_level = pngCompression;
        result = stbi_write_png(savePath.c_str(), area.width, area.height,
            channels, dataPointer, stride);

    } else {
        result = 0;
        logger.log(
            fmt::format(LOG_BAD_EXTENSION, extension),
            LogLevel::ERROR);
    }

    if (result != 0) {
        logger.log(
            fmt::format(LOG_CAPTURE_SUCCESS, savePath.c_str()),
            LogLevel::INFO);
    } else {
        logger.log(
            fmt::format(LOG_CAPTURE_FAILURE),
            LogLevel::ERROR);
    }

    return result != 0;
}

}  // namespace basil
