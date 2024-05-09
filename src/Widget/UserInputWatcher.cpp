#include "UserInputWatcher.hpp"

namespace basil {

UserInputWatcher::UserInputWatcher()
        : IBasilWidget(ProcessOrdinal::EARLY, ProcessPrivilege::LOW) {
    window = BasilContext::getGLFWWindow();
}

void UserInputWatcher::onStart() {
    // Check all button states on startup
    for (auto& button : mouseState) {
        button.second = {glfwGetMouseButton(window, button.first), 0};
    }

    glfwSetMouseButtonCallback(window, onMouseKeyChange);
}

void UserInputWatcher::onLoop() {
    if (mouseState.at(GLFW_MOUSE_BUTTON_LEFT).first > 0) {
        logger.log("Mouse button clicked!");
    }
}

void UserInputWatcher::onMouseKeyChange(
        GLFWwindow* window, int button, int action, int mods) {
    mouseState.at(button) = {action, mods};
}

}  // namespace basil
