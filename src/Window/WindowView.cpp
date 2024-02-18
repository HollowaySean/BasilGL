#include <stdio.h>

#include <chrono>
#include <thread>
#include <cmath>
#include <vector>

#include "WindowView.hpp"

// TODO(sholloway): Retrofit other optional constructors throughout project
WindowView::WindowView(std::optional<WindowProps> windowProps):
        glfwWindow() {
    this->windowProps = windowProps.value_or(WindowProps());

    topPane = nullptr;
    initializeGLFWContext();

    // Create window
    glfwWindow = createGLFWWindow();

    // GLEW can not be initialized before window context
    // has been created
    initializeGLEWContext();

    // Set window user pointer
    glfwSetWindowUserPointer(glfwWindow, this);
    setCallbacks();
}

WindowView::~WindowView() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void WindowView::onLoop() {
    if (glfwWindowShouldClose(glfwWindow)) {
        closeWindow();
        return;
    }

    draw();
}

void WindowView::onStart() {
    glfwShowWindow(glfwWindow);
}

void WindowView::onStop() {
    logger.log("Stopping loop", Level::INFO);
}

void WindowView::setTopPane(IPane* newTopPane) {
    this->topPane = newTopPane;
}

PaneProps WindowView::getTopPaneProps() {
    return PaneProps {
        .width = windowProps.width,
        .height = windowProps.height,
        .xOffset = 0,
        .yOffset = 0
    };
}

GLFWwindow* WindowView::createGLFWWindow() {
    GLFWwindow* newWindow = glfwCreateWindow(
        windowProps.width,
        windowProps.height,
        windowProps.title.c_str(),
        NULL, NULL);

    if (!newWindow) {
        logger.log("GLFW failed to create window.", Level::ERROR);
        glfwTerminate();
    } else {
        logger.log("Successfully created GLFW window.", Level::INFO);
    }

    glfwMakeContextCurrent(newWindow);

    return newWindow;
}

void WindowView::onResize(GLFWwindow* window, int width, int height) {
    if (topPane) {
        topPane->onResize(width, height);
    }
}

void WindowView::initializeGLFWContext() {
    GLenum errorCode = glfwInit();
    logGLFWError(errorCode);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Default to window not being visible
    glfwWindowHint(GLFW_VISIBLE, false);
}

void WindowView::initializeGLEWContext() {
    GLenum errorCode = glewInit();
    logGLEWError(errorCode);
}

void WindowView::logGLFWError(GLenum errorCode) {
    Logger& logger = Logger::get();

    if (errorCode) {
        logger.log("GLFW context initialized successfully.", Level::INFO);
    } else {
        const char* errorMessage;
        glfwGetError(&errorMessage);

        std::string errorMessageString = errorMessage ?
            std::string(errorMessage) : "Could not read error message.";

        logger.log("GLFW failed to initialize. Error: "
            + errorMessageString, Level::ERROR);
    }
}

void WindowView::logGLEWError(GLenum errorCode) {
    Logger& logger = Logger::get();

    if (errorCode == GLEW_OK) {
        logger.log("GLEW context initialized successfully.", Level::INFO);
    } else {
        const char* errorMessage =
            reinterpret_cast<const char*>(glewGetErrorString(errorCode));
        std::string errorMessageString = std::string(errorMessage);

        logger.log("GLEW failed to initialize. Error: "
            + errorMessageString, Level::ERROR);
    }
}

void WindowView::draw() {
    // Clear background color
    glBlendFunc(GL_ONE, GL_ZERO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render using double buffer
    if (topPane) {
        topPane->draw();
    }
    glfwSwapBuffers(glfwWindow);

    // Check for pending events
    glfwPollEvents();
}

void WindowView::closeWindow() {
    currentState = State::REQUEST_STOP;

    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void WindowView::setCallbacks() {
    auto resizeFunc = [](GLFWwindow* window, int width, int height){
        static_cast<WindowView*>(
            glfwGetWindowUserPointer(window))->onResize(window, width, height);
    };
    glfwSetFramebufferSizeCallback(glfwWindow, resizeFunc);
}
