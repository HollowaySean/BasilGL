#include <stdio.h>

#include "frame/framecontroller.hpp"

void mainLoop() {
    printf("Hello world!\n");
}

int main(int argc, char** argv) {
    FrameController frameController = FrameController(mainLoop);
    frameController.setFrameCap(1);
    frameController.start();

    return 0;
}
