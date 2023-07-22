#include "frame/framecontroller.hpp"
#include "window/windowview.hpp"

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    WindowView windowView = WindowView();

    FrameController frameController = FrameController(&windowView);
    frameController.setFrameCap(10);
    frameController.start();

    return 0;
}
