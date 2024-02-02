#include <stdio.h>

#include <chrono>
#include <thread>
#include <cmath>
#include <vector>

#include "WindowView.hpp"

WindowView::WindowView(): glfwWindow() {
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

void WindowView::onStop() {
    logger.log("Stopping loop", Level::INFO);
}

void WindowView::setTopPane(IPane* newTopPane) {
    this->topPane = newTopPane;
}

PaneProps WindowView::getTopPaneProps() {
    return PaneProps {
        .width = windowOptions.width,
        .height = windowOptions.height,
        .xOffset = 0,
        .yOffset = 0
    };
}

GLFWwindow* WindowView::createGLFWWindow() {
    GLFWwindow* newWindow = glfwCreateWindow(
        windowOptions.width,
        windowOptions.height,
        windowOptions.title.c_str(),
        NULL, NULL);

    if (!newWindow) {
        fprintf(stderr, "GLFW failed to create window.");
        glfwTerminate();
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
    if (!glfwInit()) {
        fprintf(stderr, "GLFW failed to initialize.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void WindowView::initializeGLEWContext() {
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
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
