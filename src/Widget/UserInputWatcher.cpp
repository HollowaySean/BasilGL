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

void UserInputWatcher::setCallbacks() {
    glfwSetMouseButtonCallback(window, onMouseButtonChange);
    glfwSetKeyCallback(window, onKeyChange);
    glfwSetCursorEnterCallback(window, onCursorEnter);
}

void UserInputWatcher::onMouseButtonChange(
        GLFWwindow* window, int button, int action, int mods) {
    model.setMouseButtonState(button, action, mods);
}

void UserInputWatcher::onKeyChange(
        GLFWwindow* window, int button, int scancode, int action, int mods) {
    model.setKeyState(button, action, mods);
}

void UserInputWatcher::onCursorEnter(
        GLFWwindow* window, int entered) {
    model.setIsMouseInWindow(entered > 0);
}

}  // namespace basil
