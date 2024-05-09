#include "UserInputWatcher.hpp"

namespace basil {

UserInputWatcher::UserInputWatcher()
        : IBasilWidget(ProcessOrdinal::EARLY, ProcessPrivilege::LOW) {
    window = BasilContext::getGLFWWindow();
}

void UserInputWatcher::onStart() {
    initializeState();
    setCallbacks();
}

void UserInputWatcher::onLoop() {
    checkMousePosition();

    // TODO(sholloway): upload data model
    if (model.getMouseButtonState(GLFW_MOUSE_BUTTON_LEFT).isPressed()) {
        Logger::get().log("Clicked!!!!");
    }
}

void UserInputWatcher::initializeState() {
    // Check all button states on startup
    for (uint button = 0;
            button < UserInputModel::MOUSE_BUTTONS_COUNT;
            button++) {
        int state = glfwGetMouseButton(window, button);
        model.setMouseButtonState(button, state, 0);
    }

    for (uint button = 0;
            button < UserInputModel::KEY_CODE_COUNT;
            button++) {
        int state = glfwGetKey(window, button);
        model.setKeyState(button, state, 0);
    }

    // Check mouse state on startup
    checkMousePosition();
    checkIsMouseInWindow();
}

void UserInputWatcher::checkMousePosition() {
    double xPosition, yPosition;
    glfwGetCursorPos(window, &xPosition, &yPosition);

    model.setMousePosition(xPosition, yPosition);
}

void UserInputWatcher::checkIsMouseInWindow() {
    bool isInWindow = glfwGetWindowAttrib(window, GLFW_HOVERED);

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
