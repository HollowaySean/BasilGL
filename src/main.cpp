#include "Frame.hpp"
#include "Window.hpp"

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    WindowView windowView = WindowView();
    FrameController frameController = FrameController();

    frameController.addProcess(&windowView);
    frameController.setFrameCap(20);
    frameController.start();

    return 0;
}
