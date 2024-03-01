#include <stdio.h>

#include <chrono>
#include <thread>
#include <cmath>
#include <vector>

#include "WindowView.hpp"

namespace basil {

WindowView::WindowView(std::optional<WindowProps> windowProps) {
    this->windowProps = windowProps.value_or(WindowProps());

    // Create window
    glfwWindow = BasilContext::getGLFWWindow();

    // Set window properties
    setWindowSize(this->windowProps.width, this->windowProps.height);
    setWindowTitle(this->windowProps.title);

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
    PaneProps propsFromWindow = getTopPaneProps();
    newTopPane->setPaneProps(propsFromWindow);

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

void WindowView::setWindowSize(int width, int height) {
    this->windowProps.width = width;
    this->windowProps.height = height;

    glfwSetWindowSize(glfwWindow, width, height);
}

void WindowView::setWindowTitle(const std::string& title) {
    this->windowProps.title = title;

    glfwSetWindowTitle(glfwWindow, title.c_str());
}

void WindowView::setWindowProps(WindowProps newWindowProps) {
    setWindowSize(newWindowProps.width, newWindowProps.height);
    setWindowTitle(newWindowProps.title);
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

WindowView::Builder&
WindowView::Builder::withDimensions(int width, int height) {
    impl->setWindowSize(width, height);
    return (*this);
}

WindowView::Builder&
WindowView::Builder::withTitle(const std::string& title) {
    impl->setWindowTitle(title);
    return (*this);
}

WindowView::Builder&
WindowView::Builder::withTopPane(std::shared_ptr<IPane> topPane) {
    impl->setTopPane(topPane);
    return (*this);
}

}  // namespace basil
