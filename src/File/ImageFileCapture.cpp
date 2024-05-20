#include "ImageFileCapture.hpp"

#include <fmt/format.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace basil {

ImageFileCapture::ImageFileCapture() {
    glGenBuffers(1, &pixelBufferID);
    // TODO(sholloway): logging

    GLFWwindow* window = BasilContext::getGLFWWindow();
    glfwGetFramebufferSize(window, &width, &height);
    updateBufferSize(width, height);

    using std::placeholders::_1, std::placeholders::_2;
    BasilContext::setGLFWFramebufferSizeCallback(
        std::bind(&ImageFileCapture::updateBufferSize, this, _1, _2));
}

ImageFileCapture::~ImageFileCapture() {
    glDeleteBuffers(1, &pixelBufferID);
    // TODO(sholloway): logging
}

void ImageFileCapture::updateBufferSize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    int bytes = 3 * width * height;

    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
    glBufferData(GL_PIXEL_PACK_BUFFER, bytes, NULL, GL_STREAM_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

bool ImageFileCapture::capture(
        std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea ) {
    // Obtain window size if capture area not provided
    auto area = captureArea.value_or(
        ImageCaptureArea { width, height, 0, 0 });

    // Move pixel data into PBO
    auto pixelDataPointer = copyFrameToBuffer();

    // Write to PNG
    bool success = saveBufferToFile(pixelDataPointer, area, savePath);

    return success;
}

std::future<bool> ImageFileCapture::captureAsync(
        std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea) {
    auto area = captureArea.value_or(
        ImageCaptureArea { width, height, 0, 0 });

    // Move pixel data into PBO
    auto pixelDataPointer = copyFrameToBuffer();

    // Dispatch write-to-file process
    auto saveProcess = [this, pixelDataPointer, area, savePath] {
        return saveBufferToFile(pixelDataPointer, area, savePath);
    };
    auto future = std::async(std::launch::async, saveProcess);

    return future;
}

GLubyte* ImageFileCapture::copyFrameToBuffer() {
    // Bind buffer and copy
    glReadBuffer(GL_FRONT);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, 0);

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

    // Write to PNG
    bool success = false;
    if (dataPointer) {
        stbi_flip_vertically_on_write(true);
        int result = stbi_write_png(
            savePath.c_str(), area.width, area.height,
            channels, dataPointer, stride);
        success = result != 0;

        // TODO(sholloway): Move this to a cleanup function
        // glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    } else {
        logger.log("ERROR HERE");
        // TODO(sholloway): logging
    }
    // TODO(sholloway): Move this to a cleanup function
    // glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    if (success) {
        logger.log(
            fmt::format(LOG_CAPTURE_SUCCESS, savePath.c_str()),
            LogLevel::INFO);
    } else {
        logger.log(
            fmt::format(LOG_CAPTURE_FAILURE),
            LogLevel::ERROR);
    }

    return success;
}

}  // namespace basil
