#ifndef SRC_DATA_USERINPUTMODEL_HPP_
#define SRC_DATA_USERINPUTMODEL_HPP_

#include <utility>
#include <array>

namespace basil {

// TODO(sholloway): Documentation
class UserInputModel {
 public:
    struct KeyState {
        int pressState;
        int modifiers;

        bool isPressed() {
            return pressState > 0;
        }
    };

    struct MousePosition {
        double xPosition;
        double yPosition;
        bool isInWindow;
    };

    struct WindowSize {
        int width;
        int height;
    };

    KeyState getKeyState(uint keyCode) {
        return keyStates.at(keyCode);
    }

    bool getIsKeyPressed(uint keyCode) {
        return getKeyState(keyCode).isPressed();
    }

    void setKeyState(uint keyCode, int state, int mods) {
        keyStates.at(keyCode) = { state, mods };
    }

    KeyState getMouseButtonState(uint buttonCode) {
        return mouseStates.at(buttonCode);
    }

    bool getIsMouseButtonPressed(uint buttonCode) {
        return getMouseButtonState(buttonCode).isPressed();
    }

    MousePosition getMousePosition() {
        return mousePosition;
    }

    bool getIsMouseInWindow() {
        return mousePosition.isInWindow;
    }

    void setMouseButtonState(uint buttonCode, int state, int mods) {
        mouseStates.at(buttonCode) = { state, mods };
    }

    void setMousePosition(double xPosition, double yPosition) {
        mousePosition.xPosition = xPosition;
        mousePosition.yPosition = yPosition;
    }

    void setIsMouseInWindow(bool isMouseInWindow) {
        mousePosition.isInWindow = isMouseInWindow;
    }

    WindowSize getWindowSize() {
        return windowSize;
    }

    void setWindowSize(int width, int height) {
        windowSize.width = width;
        windowSize.height = height;
    }

    static inline const uint MOUSE_BUTTONS_COUNT = 8;
    static inline const uint KEY_CODE_COUNT = 349;

#ifndef TEST_BUILD

 private:
#endif
    std::array<KeyState, MOUSE_BUTTONS_COUNT>
        mouseStates = { { {-1, 0} } };
    std::array<KeyState, KEY_CODE_COUNT>
        keyStates   = { { {-1, 0} } };

    MousePosition mousePosition = { 0., 0., false };
    WindowSize windowSize = { 0, 0 };
};

}  // namespace basil

#endif  // SRC_DATA_USERINPUTMODEL_HPP_
