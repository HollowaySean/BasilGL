#include "WindowView.hpp"

namespace basil {

WindowView::WindowView(std::optional<WindowProps> windowProps) : IBasilWidget({
        "WindowView",
        ProcessOrdinal::MAIN,
        ProcessPrivilege::HIGH,
        WidgetPubSubPrefs::SUBSCRIBE_ONLY
    }) {
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

void WindowView::receiveData(const DataMessage& message) {
    publishData(message);
}

void WindowView::setTopPane(std::shared_ptr<IPane> newTopPane) {
    if (topPane) {
        IDataPublisher::unsubscribe(topPane);
    }

    ViewArea propsFromWindow = getTopPaneProps();
    newTopPane->setPaneProps(propsFromWindow);

    this->topPane = newTopPane;

    IDataPublisher::subscribe(topPane);
}

ViewArea WindowView::getTopPaneProps() {
    return ViewArea {
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

void WindowView::setWindowProps(const WindowProps& newWindowProps) {
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
    setCurrentState(ProcessState::REQUEST_STOP);
    removeCallbacks();

    glfwDestroyWindow(glfwWindow);
}

void WindowView::setCallbacks() {
    using std::placeholders::_1;
    using std::placeholders::_2;

    BasilContext::BasilFrameBufferSizeFunc func =
        std::bind(&WindowView::onResize, this, _1, _2);
    resizeCallbackID =
        BasilContext::setGLFWFramebufferSizeCallback(func);
}

void WindowView::removeCallbacks() {
    BasilContext::removeGLFWFramebufferSizeCallback(resizeCallbackID);
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
