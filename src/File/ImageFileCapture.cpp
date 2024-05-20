#include "ImageFileCapture.hpp"

#include <fmt/format.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace basil {

bool ImageFileCapture::capture(
        std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea ) {
    // Verify that a rendering context exists already
    if (!BasilContext::isInitialized()) {
        return false;
    }

    // Obtain window size if capture area not provided
    ImageCaptureArea area;
    if (captureArea.has_value()) {
        area = captureArea.value();
    } else {
        area = getWindowCaptureArea();
    }

    // Calculate buffer size and enforce multiple of 4
    GLsizei channels = 3;
    GLsizei stride = channels * area.width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * area.height;

    GLFWwindow* window = BasilContext::getGLFWWindow();
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);

    if (pixelBufferID == -1) {
        initializeBuffer();
    }

    // Bind buffer and copy
    glReadBuffer(GL_FRONT);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, 0);

    GLubyte* pixelDataPointer = static_cast<GLubyte*>(
        glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));



    // Write to PNG
    bool success = false;
    if (pixelDataPointer) {
        stbi_flip_vertically_on_write(true);
        int result = stbi_write_png(
            savePath.c_str(), area.width, area.height,
            channels, pixelDataPointer, stride);
        success = result != 0;

        glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    } else {
        logger.log("ERROR HERE");
    }
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

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

std::future<bool> ImageFileCapture::captureAsync(
        std::filesystem::path savePath,
        std::optional<ImageCaptureArea> captureArea) {
    // Verify that a rendering context exists already
    if (!BasilContext::isInitialized()) {
        auto promise = std::promise<bool>();
        promise.set_value(false);
        return promise.get_future();
    }

    // Obtain window size if capture area not provided
    ImageCaptureArea area;
    if (captureArea.has_value()) {
        area = captureArea.value();
    } else {
        area = getWindowCaptureArea();
    }

    if (pixelBufferID == -1) {
        initializeBuffer();
    }

    GLFWwindow* window = BasilContext::getGLFWWindow();
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);

    if (pixelBufferID == -1) {
        initializeBuffer();
    }

    // Bind buffer and copy
    glReadBuffer(GL_FRONT);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, 0);

    GLubyte* pixelDataPointer = static_cast<GLubyte*>(
        glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));

    auto saveProcess = [this, pixelDataPointer, area, savePath] {
        return saveBufferToFile(pixelDataPointer, area, savePath);
    };
    auto future = std::async(std::launch::async, saveProcess);

    return future;
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

ImageCaptureArea ImageFileCapture::getWindowCaptureArea() {
    GLFWwindow* window = BasilContext::getGLFWWindow();
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);

    return { width, height, 0, 0 };
}

void ImageFileCapture::initializeBuffer() {
    glGenBuffers(1, &pixelBufferID);

    GLFWwindow* window = BasilContext::getGLFWWindow();
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);
    int bytes = 3 * width * height;

    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferID);
    glBufferData(GL_PIXEL_PACK_BUFFER, bytes, NULL, GL_STREAM_READ);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}

}  // namespace basil
