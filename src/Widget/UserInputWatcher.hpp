#ifndef SRC_WIDGET_USERINPUTWATCHER_HPP_
#define SRC_WIDGET_USERINPUTWATCHER_HPP_

#include <map>
#include <utility>

#include <Basil/App.hpp>

#include "Data/UserInputModel.hpp"

namespace basil {

// TODO(sholloway): Documentation
class UserInputWatcher : public  IBasilWidget,
                         public  IBuildable<UserInputWatcher>,
                         private IBasilContextConsumer {
 public:
    UserInputWatcher();

    void onStart() override;

    void onLoop() override;

    UserInputModel& getModel() { return model; }

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

}  // namespace basil

#endif  // SRC_WIDGET_USERINPUTWATCHER_HPP_
