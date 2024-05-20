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

    GLFWwindow* window = BasilContext::getGLFWWindow();
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // TODO(sholloway): Use pixel buffer object instead

    // Bind framebuffers and copy
    glBlitNamedFramebuffer(
        0, framebufferID,
        0, 0, width, height,
        0, 0, width, height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // Read from framebuffer
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glNamedFramebufferReadBuffer(framebufferID, GL_COLOR_ATTACHMENT0);
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

void ImageFileCapture::initializeFramebuffer() {
    glGenFramebuffers(1, &framebufferID);
    // glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

    // glGenTextures(1, &textureID);
    // glBindTexture(GL_TEXTURE_2D, textureID);

    GLFWwindow* window = BasilContext::getGLFWWindow();
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
    //     0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
    //     GL_TEXTURE_2D, textureID, 0);

    glGenRenderbuffers(1, &renderbufferID);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB8, width, height);

    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_RENDERBUFFER, renderbufferID);

    // if(glCheckFramebufferStatus(GL_FRAMEBUFFER))
}

}  // namespace basil
