#ifndef SRC_WINDOW_WINDOWVIEW_HPP_
#define SRC_WINDOW_WINDOWVIEW_HPP_

#include "../frame/framecontroller.hpp"

class WindowView: public Runnable {
 public:
    WindowView();
    void mainLoop() override;
};

#endif  // SRC_WINDOW_WINDOWVIEW_HPP_
