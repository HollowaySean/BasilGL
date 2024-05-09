#ifndef SRC_WIDGET_USERINPUTWATCHER_HPP_
#define SRC_WIDGET_USERINPUTWATCHER_HPP_

#include <map>
#include <utility>

#include <Basil/App.hpp>

namespace basil {

class UserInputWatcher : public  IBasilWidget,
                         public  IBuildable<UserInputWatcher>,
                         private IBasilContextConsumer {
 public:
    UserInputWatcher();

    void onStart() override;

    void onLoop() override;

 private:
    // TEMP
    Logger& logger = Logger::get();

    GLFWwindow* window;
    // TODO(sholloway): Break out into data structure after PoC
    static inline std::map<int, std::pair<int, int>> mouseState = {
            {GLFW_MOUSE_BUTTON_LEFT,    {GLFW_KEY_UNKNOWN, 0}},
            {GLFW_MOUSE_BUTTON_MIDDLE,  {GLFW_KEY_UNKNOWN, 0}},
            {GLFW_MOUSE_BUTTON_RIGHT,   {GLFW_KEY_UNKNOWN, 0}}
        };

    static void onMouseKeyChange(
        GLFWwindow* window, int button, int action, int mods);
};

}  // namespace basil

#endif  // SRC_WIDGET_USERINPUTWATCHER_HPP_
