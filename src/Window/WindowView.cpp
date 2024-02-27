#include <stdio.h>

#include <chrono>
#include <thread>
#include <cmath>
#include <vector>

#include "WindowView.hpp"

namespace basil {

WindowView::WindowView(std::optional<WindowProps> windowProps):
        glfwWindow() {
    this->windowProps = windowProps.value_or(WindowProps());

    // Create window
    glfwWindow = createGLFWWindow();

    // Set window user pointer
    glfwSetWindowUserPointer(glfwWindow, this);
    setCallbacks();
}

WindowView::~WindowView() {
    closeWindow();
}

void WindowView::onLoop() {
    if (glfwWindowShouldClose(glfwWindow)) {
        closeWindow();
        return;
    }

    draw();
}

void WindowView::onStart() {
    #ifdef TEST_BUILD
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwIconifyWindow(glfwWindow);
    #endif
    glfwShowWindow(glfwWindow);
}

void WindowView::onStop() {
    logger.log("Stopping loop", LogLevel::INFO);
}

void WindowView::setTopPane(std::shared_ptr<IPane> newTopPane) {
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

void WindowView::setWindowProps(WindowProps newWindowProps) {
    this->windowProps = newWindowProps;

    glfwSetWindowTitle(glfwWindow, newWindowProps.title.c_str());
    glfwSetWindowSize(glfwWindow, newWindowProps.width, newWindowProps.height);
}

GLFWwindow* WindowView::createGLFWWindow() {
    GLFWwindow* newWindow = glfwCreateWindow(
        windowProps.width,
        windowProps.height,
        windowProps.title.c_str(),
        NULL, NULL);

    if (!newWindow) {
        logger.log("GLFW failed to create window.", LogLevel::ERROR);

        glfwTerminate();
        BasilContext::terminate();

        return nullptr;
    }

    logger.log("Successfully created GLFW window.", LogLevel::INFO);

    glfwMakeContextCurrent(newWindow);

    return newWindow;
}

void WindowView::onResize(int width, int height) {
    windowProps.width = width;
    windowProps.height = height;

    if (topPane) {
        topPane->onResize(width, height);
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

    BasilContext::terminate();
}

void WindowView::resizeCallback(GLFWwindow* window, int width, int height) {
    WindowView* windowView =
        static_cast<WindowView*>(glfwGetWindowUserPointer(window));
    windowView->onResize(width, height);
}

void WindowView::setCallbacks() {
    glfwSetFramebufferSizeCallback(glfwWindow, WindowView::resizeCallback);
}

}  // namespace basil
