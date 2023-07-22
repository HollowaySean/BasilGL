#ifndef SRC_WINDOW_WINDOWVIEW_HPP_
#define SRC_WINDOW_WINDOWVIEW_HPP_

#include "../frame/framecontroller.hpp"

/**
 * @brief Outer window containing all UI elements and providing simple
 * public facade.
 */
class WindowView: public Runnable {
 public:
    /** @brief Create window environment and launch window
     *  TODO: Separate launch into new method.
    */
    WindowView();

    void onStart() override;
    void onStop() override;

    /** @brief Main loop function for Runnable parent class. */
    void mainLoop() override;

 private:
    int counter = 1;
};

#endif  // SRC_WINDOW_WINDOWVIEW_HPP_
