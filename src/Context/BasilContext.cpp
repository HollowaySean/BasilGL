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

    // Set callback functions
    setGLFWCallbacks();
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

void BasilContext::setGLFWCallbacks() {
    GLFWwindow* window = getGLFWWindow();

    glfwSetFramebufferSizeCallback(window, BasilContext::onFrameBufferResize);
    glfwSetMouseButtonCallback(window, BasilContext::onMouseButton);
    glfwSetKeyCallback(window, BasilContext::onKeyAction);
    glfwSetCursorEnterCallback(window, BasilContext::onCursorEnter);
}

void BasilContext::onFrameBufferResize(
        GLFWwindow* window, int width, int height) {
    for (auto callback : framebufferCallbacks) {
        callback(width, height);
    }
}

void BasilContext::onMouseButton(
        GLFWwindow* window, int button, int action, int mods) {
    for (auto callback : mouseButtonCallbacks) {
        callback(button, action, mods);
    }
}

void BasilContext::onKeyAction(
        GLFWwindow* window, int key, int scancode, int action, int mods) {
    for (auto callback : keyCallbacks) {
        callback(key, scancode, action, mods);
    }
}

void BasilContext::onCursorEnter(
        GLFWwindow* window, int entered) {
    for (auto callback : cursorEnterCallbacks) {
        callback(entered);
    }
}

void BasilContext::setGLFWFramebufferSizeCallback(
        const BasilFrameBufferSizeFunc& callback) {
    framebufferCallbacks.push_back(callback);
}

void BasilContext::setGLFWMouseButtonCallback(
        const BasilMouseButtonFunc& callback) {
    mouseButtonCallbacks.push_back(callback);
}

void BasilContext::setGLFWKeyCallback(
        const BasilKeyFunc& callback) {
    keyCallbacks.push_back(callback);
}

void BasilContext::setGLFWCursorEnterCallback(
        const BasilCursorEnterFunc& callback) {
    cursorEnterCallbacks.push_back(callback);
}

}  // namespace basil
