#ifndef SRC_DATA_USERINPUTMODEL_HPP_
#define SRC_DATA_USERINPUTMODEL_HPP_

#include <utility>
#include <array>

#include "IDataModel.hpp"

namespace basil {

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

class UserInputModel : public IDataModel {
 public:
    KeyState getKeyState(uint keyCode) {
        return keyStates.at(keyCode);
    }

    KeyState getMouseButtonState(uint buttonCode) {
        return mouseStates.at(buttonCode);
    }

    MousePosition getMousePosition() {
        return mousePosition;
    }

    void setMousePosition(double xPosition, double yPosition) {
        mousePosition.xPosition = xPosition;
        mousePosition.yPosition = yPosition;
    }

    void setIsMouseInWindow(bool isMouseInWindow) {
        mousePosition.isInWindow = isMouseInWindow;
    }
    void setKeyState(uint keyCode, int state, int mods) {
        keyStates.at(keyCode) = { state, mods };
    }

    void setMouseButtonState(uint buttonCode, int state, int mods) {
        mouseStates.at(buttonCode) = { state, mods };
    }

    static inline const uint MOUSE_BUTTONS_COUNT = 8;
    static inline const uint KEY_CODE_COUNT = 349;

 private:
    std::array<KeyState, MOUSE_BUTTONS_COUNT>
        mouseStates = { { {-1, 0} } };
    std::array<KeyState, KEY_CODE_COUNT>
        keyStates   = { { {-1, 0} } };

    MousePosition mousePosition = { 0., 0., false };
};

}  // namespace basil

#endif  // SRC_DATA_USERINPUTMODEL_HPP_
