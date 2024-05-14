#ifndef SRC_WIDGET_USERINPUTWATCHER_HPP_
#define SRC_WIDGET_USERINPUTWATCHER_HPP_

#include <map>
#include <utility>

#include <Basil/App.hpp>

#include "Data/UserInputModel.hpp"

namespace basil {

// TODO(sholloway): Add screen size watcher?
// TODO(sholloway): Documentation
class UserInputWatcher : public  IBasilWidget,
                         public  IBuildable<UserInputWatcher>,
                         private IBasilContextConsumer {
 public:
    UserInputWatcher();

    void onStart() override;

    void onLoop() override;

#ifndef TEST_BUILD

 private:
#endif
    GLFWwindow* window;

    void initializeState();
    void setCallbacks();
    void checkMousePosition();
    void checkIsMouseInWindow();

    static inline UserInputModel model = UserInputModel();

    static void onMouseButtonChange(
        GLFWwindow* window, int button, int action, int mods);
    static void onKeyChange(
        GLFWwindow* window, int button, int scancode, int action, int mods);
    static void onCursorEnter(
        GLFWwindow* window, int entered);

    #ifdef TEST_BUILD
    static inline const double TEST_MOUSE_X_POSITION = 1;
    static inline const double TEST_MOUSE_Y_POSITION = 2;
    static inline const bool TEST_MOUSE_IS_IN_WINDOW = true;
    #endif
};

}  // namespace basil

#endif  // SRC_WIDGET_USERINPUTWATCHER_HPP_
