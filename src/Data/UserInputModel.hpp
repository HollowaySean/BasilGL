#pragma once

#include <array>

namespace basil {

/** @brief Container for mouse and keyboard input state */
class UserInputModel {
 public:
    /** @brief Struct to hold the state of a single key or button */
    struct KeyState {
        /** @brief Indicates either GLFW_KEY_UNKNOWN, GLFW_RELEASE,
         *  GLFW_PRESS, or GLFW_REPEAT */
        int pressState;

        /** @brief Bitmask of GLFW modifier keys */
        int modifiers;

        /** @returns Whether key is currently pressed */
        bool isPressed() {
            return pressState > 0;
        }
    };

    /** @brief Struct to hold the position of the mouse cursor */
    struct MousePosition {
        /** @brief Horizontal position, in screen coordinates */
        double xPosition;

        /** @brief Vertical position, in screen coordinates */
        double yPosition;

        /** @brief Whether cursor is within the GLFW window */
        bool isInWindow;
    };

    /** @brief Struct containing window width & height */
    struct WindowSize {
        /** @brief Window width, in pixels */
        int width;

        /** @brief Window height, in pixels */
        int height;
    };

    /** @returns State of key with keyCode */
    KeyState getKeyState(uint keyCode) {
        return keyStates.at(keyCode);
    }

    /** @returns Whether key with keycode is pressed */
    bool getIsKeyPressed(uint keyCode) {
        return getKeyState(keyCode).isPressed();
    }

    /** @brief Sets state of key with keyCode */
    void setKeyState(uint keyCode, int state, int mods) {
        keyStates.at(keyCode) = { state, mods };
    }

    /** @returns State of mouse button with buttonCode */
    KeyState getMouseButtonState(uint buttonCode) {
        return mouseStates.at(buttonCode);
    }

    /** @returns Whether mouse button with buttonCode is pressed */
    bool getIsMouseButtonPressed(uint buttonCode) {
        return getMouseButtonState(buttonCode).isPressed();
    }

    /** @returns Mouse position struct */
    MousePosition getMousePosition() {
        return mousePosition;
    }

    /** @returns Whether mouse is within GLFW window */
    bool getIsMouseInWindow() {
        return mousePosition.isInWindow;
    }

    /** @brief Sets state of mouse button with buttonCode */
    void setMouseButtonState(uint buttonCode, int state, int mods) {
        mouseStates.at(buttonCode) = { state, mods };
    }

    /** @brief Sets position of cursor */
    void setMousePosition(double xPosition, double yPosition) {
        mousePosition.xPosition = xPosition;
        mousePosition.yPosition = yPosition;
    }

    /** @brief Sets whether cursor is within GLFW window */
    void setIsMouseInWindow(bool isMouseInWindow) {
        mousePosition.isInWindow = isMouseInWindow;
    }

    /** @returns Window size struct */
    WindowSize getWindowSize() {
        return windowSize;
    }

    /** @brief Sets width and height of window */
    void setWindowSize(int width, int height) {
        windowSize.width = width;
        windowSize.height = height;
    }

    /** @brief Number of buttons in mouse button state map */
    static inline const uint MOUSE_BUTTONS_COUNT = 8;

    /** @brief Number of keys in keyboard state map */
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

}   // namespace basil
