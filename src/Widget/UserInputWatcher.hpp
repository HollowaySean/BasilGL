#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Basil/Packages/App.hpp>
#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/Context.hpp>

#include "Data/UserInputModel.hpp"

namespace basil {

/** @brief IBasilWidget which monitors and publishes user inputs */
class UserInputWatcher : public  IBasilWidget,
                         public  IBuildable<UserInputWatcher>,
                         private IBasilContextConsumer {
 public:
    /** @brief Initialize UserInputWatcher */
    UserInputWatcher();

    /** @brief Deinitialize UserInputWatcher */
    ~UserInputWatcher();

    /** @brief Initialize input state model */
    void onStart() override;

    /** @brief Update mouse position and publish state */
    void onLoop() override;

    /** @brief De-register callbacks. */
    void onStop() override;

    /** @returns Reference to UserInputModel */
    UserInputModel& getModel() { return model; }

    /** @brief Builder pattern for UserInputWatcher */
    class Builder : public IBuildable<UserInputWatcher> {};

#ifndef TEST_BUILD

 private:
#endif
    GLFWwindow* window;

    void initializeState();
    void setCallbacks();
    void removeCallbacks();
    void checkMousePosition();
    void checkIsMouseInWindow();
    void checkWindowSize();

    UserInputModel model = UserInputModel();

    void onMouseButtonChange(
        int button, int action, int mods);
    unsigned int mouseCallbackID = -1;
    void onKeyChange(
        int button, int scancode, int action, int mods);
    unsigned int keyCallbackID = -1;
    void onCursorEnter(
        int entered);
    unsigned int cursorCallbackID = -1;
    void onResize(
        int width, int height);
    unsigned int resizeCallbackID = -1;

    #ifdef TEST_BUILD
    static inline const double TEST_MOUSE_X_POSITION = 1;
    static inline const double TEST_MOUSE_Y_POSITION = 2;
    static inline const bool TEST_MOUSE_IS_IN_WINDOW = true;
    static inline const int TEST_WINDOW_WIDTH = 2;
    static inline const int TEST_WINDOW_HEIGHT = 1;
    #endif
};

}   // namespace basil
