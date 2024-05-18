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

    // Allocate space for image read
    std::vector<char> buffer(bufferSize);

    // Read from framebuffer
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(
        area.xOffset, area.yOffset, area.width, area.height,
        GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    // Write to PNG
    stbi_flip_vertically_on_write(true);
    int result = stbi_write_png(
        savePath.c_str(), area.width, area.height,
        channels, buffer.data(), stride);
    bool success = result != 0;

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

}  // namespace basil
