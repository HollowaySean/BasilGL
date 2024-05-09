#ifndef SRC_WIDGET_USERINPUTWATCHER_HPP_
#define SRC_WIDGET_USERINPUTWATCHER_HPP_

#include <map>
#include <utility>

#include <Basil/App.hpp>

#include "Data/UserInputModel.hpp"

namespace basil {

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
};

}  // namespace basil

#endif  // SRC_WIDGET_USERINPUTWATCHER_HPP_
