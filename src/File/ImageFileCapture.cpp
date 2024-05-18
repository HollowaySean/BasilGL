#include "ImageFileCapture.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace basil {

bool ImageFileCapture::capture(std::filesystem::path savePath) {
    if (!BasilContext::isInitialized()) {
        return false;
    }

    // Log info message
    Logger& logger = Logger::get();
    logger.log("Saving to file");
    // TODO(sholloway): Better log messages

    // Get size of current window
    GLFWwindow* window = BasilContext::getGLFWWindow();
    GLint width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Calculate buffer size and enforce multiple of 4
    GLsizei channels = 3;
    GLsizei stride = channels * width;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei bufferSize = stride * height;

    // Allocate space for image read
    std::vector<char> buffer(bufferSize);

    // Read from framebuffer
    // TODO(sholloway): Figure out what this does
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

    // Write to PNG
    stbi_flip_vertically_on_write(true);
    int result = stbi_write_png(
        savePath.c_str(), width, height, channels, buffer.data(), stride);

    return result != 0;
}

}  // namespace basil
