#include "frame/framecontroller.hpp"
#include "window/windowview.hpp"

int main(int argc, char** argv) {
    WindowView windowView = WindowView();

    FrameController frameController = FrameController(&windowView);
    frameController.setFrameCap(1);
    frameController.start();

    return 0;
}
