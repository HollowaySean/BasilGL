#pragma once

#include <map>
#include <utility>

#include <Basil/App.hpp>
#include <Basil/Context.hpp>

#include "Data/UserInputModel.hpp"

namespace basil {

/** @brief IBasilWidget which monitors and publishes user inputs */
class UserInputWatcher : public  IBasilWidget,
                         public  IBuildable<UserInputWatcher>,
                         private IBasilContextConsumer {
 public:
    /** @brief Initialize UserInputWatcher */
    UserInputWatcher();

    /** @brief Initialize input state model */
    void onStart() override;

    /** @brief Update mouse position and publish state */
    void onLoop() override;

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
    void checkMousePosition();
    void checkIsMouseInWindow();
    void checkWindowSize();

    UserInputModel model = UserInputModel();

    void onMouseButtonChange(
        int button, int action, int mods);
    void onKeyChange(
        int button, int scancode, int action, int mods);
    void onCursorEnter(
        int entered);
    void onResize(
        int width, int height);

    #ifdef TEST_BUILD
    static inline const double TEST_MOUSE_X_POSITION = 1;
    static inline const double TEST_MOUSE_Y_POSITION = 2;
    static inline const bool TEST_MOUSE_IS_IN_WINDOW = true;
    static inline const int TEST_WINDOW_WIDTH = 2;
    static inline const int TEST_WINDOW_HEIGHT = 1;
    #endif
};

}   // namespace basil
