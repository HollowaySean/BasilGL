#include "UserInputWatcher.hpp"

namespace basil {

UserInputWatcher::UserInputWatcher()
        : IBasilWidget(ProcessOrdinal::EARLY,
            ProcessPrivilege::LOW,
            "UserInputWatcher") {
    window = BasilContext::getGLFWWindow();
}

void UserInputWatcher::onStart() {
    initializeState();
    setCallbacks();
}

void UserInputWatcher::onLoop() {
    checkMousePosition();

    this->IDataPublisher::publishData(DataMessage(model));
}

void UserInputWatcher::initializeState() {
    // Check all button states on startup
    for (uint button = 0;
            button < UserInputModel::MOUSE_BUTTONS_COUNT;
            button++) {
        #ifndef TEST_BUILD
        int state = glfwGetMouseButton(window, button);
        #else
        int state = GLFW_PRESS;
        #endif

        model.setMouseButtonState(button, state, 0);
    }

    for (uint button = 0;
            button < UserInputModel::KEY_CODE_COUNT;
            button++) {
        #ifndef TEST_BUILD
        int state = glfwGetKey(window, button);
        #else
        int state = GLFW_PRESS;
        #endif
        model.setKeyState(button, state, 0);
    }

    // Check mouse state on startup
    checkMousePosition();
    checkIsMouseInWindow();

    // Check window size on startup
    checkWindowSize();
}

void UserInputWatcher::checkMousePosition() {
    double xPosition, yPosition;

    #ifndef TEST_BUILD
    glfwGetCursorPos(window, &xPosition, &yPosition);
    #else
    xPosition = TEST_MOUSE_X_POSITION;
    yPosition = TEST_MOUSE_Y_POSITION;
    #endif

    model.setMousePosition(xPosition, yPosition);
}

void UserInputWatcher::checkIsMouseInWindow() {
    #ifndef TEST_BUILD
    bool isInWindow = glfwGetWindowAttrib(window, GLFW_HOVERED);
    #else
    bool isInWindow = TEST_MOUSE_IS_IN_WINDOW;
    #endif

    model.setIsMouseInWindow(isInWindow);
}

void UserInputWatcher::checkWindowSize() {
    #ifndef TEST_BUILD
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    #else
    int width = TEST_WINDOW_WIDTH;
    int height = TEST_WINDOW_HEIGHT;
    #endif

    model.setWindowSize(width, height);
}

void UserInputWatcher::setCallbacks() {
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    using std::placeholders::_4;

    BasilContext::setGLFWMouseButtonCallback(
        std::bind(&UserInputWatcher::onMouseButtonChange, this, _1, _2, _3));
    BasilContext::setGLFWKeyCallback(
        std::bind(&UserInputWatcher::onKeyChange, this, _1, _2, _3, _4));
    BasilContext::setGLFWCursorEnterCallback(
        std::bind(&UserInputWatcher::onCursorEnter, this, _1));
    BasilContext::setGLFWFramebufferSizeCallback(
        std::bind(&UserInputWatcher::onResize, this, _1, _2));
}

void UserInputWatcher::onMouseButtonChange(
        int button, int action, int mods) {
    model.setMouseButtonState(button, action, mods);
}

void UserInputWatcher::onKeyChange(
        int button, int scancode, int action, int mods) {
    model.setKeyState(button, action, mods);
}

void UserInputWatcher::onCursorEnter(
        int entered) {
    model.setIsMouseInWindow(entered > 0);
}

void UserInputWatcher::onResize(
        int width, int height) {
    model.setWindowSize(width, height);
}

}  // namespace basil
