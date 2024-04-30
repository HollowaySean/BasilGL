#include <string>

#include "BasilContext.hpp"

namespace basil {

void BasilContext::initialize() {
    BasilContext& instance = get();

    if (instance.isInitialized()) return;

    hasInitialized = true;
    instance.initializeGLFWContext();
    instance.initializeGLEWContext();
}

void BasilContext::terminate() {
    glfwTerminate();
    hasInitialized = false;
}

void BasilContext::initializeGLFWContext() {
    // Initialize log errors, and update flag
    GLenum errorCode = glfwInit();
    logGLFWError(errorCode);
    hasInitialized &= errorCode;

    // Create non-visible window and attach context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_VISIBLE, false);
    glfwWindow = glfwCreateWindow(
        1, 1, WINDOW_TITLE, NULL, NULL);
    glfwMakeContextCurrent(glfwWindow);

    // Save success/failure flag and log errors
    hasInitialized &= glfwWindow != nullptr;
    logGLFWWindowError(glfwWindow);
}

void BasilContext::initializeGLEWContext() {
    GLenum errorCode = glewInit();
    logGLEWError(errorCode);

    // Save success/failure flag
    hasInitialized &= !(errorCode);
}

void BasilContext::logGLFWError(GLenum errorCode) {
    Logger& logger = Logger::get();

    if (errorCode) {
        logger.log("GLFW context initialized successfully.", LogLevel::INFO);
    } else {
        const char* errorMessage;
        glfwGetError(&errorMessage);

        std::string errorMessageString = errorMessage ?
            std::string(errorMessage) : "Could not read error message.";

        logger.log("GLFW failed to initialize. Error: "
            + errorMessageString, LogLevel::ERROR);
    }
}

void BasilContext::logGLFWWindowError(const GLFWwindow* window) {
    Logger& logger = Logger::get();

    if (window) {
        logger.log("GLFW window created successfully.", LogLevel::INFO);
    } else {
        logger.log("GLFW failed to create window.", LogLevel::ERROR);
        BasilContext::terminate();
    }
}

void BasilContext::logGLEWError(GLenum errorCode) {
    Logger& logger = Logger::get();

    if (errorCode == GLEW_OK) {
        logger.log("GLEW context initialized successfully.", LogLevel::INFO);
    } else {
        const char* errorMessage =
            reinterpret_cast<const char*>(glewGetErrorString(errorCode));
        std::string errorMessageString = std::string(errorMessage);

        logger.log("GLEW failed to initialize. Error: "
            + errorMessageString, LogLevel::ERROR);
    }
}

GLFWwindow* BasilContext::getGLFWWindow() {
    if (!hasInitialized) {
        initialize();
    }

    const BasilContext& instance = get();
    return instance.glfwWindow;
}

}  // namespace basil
