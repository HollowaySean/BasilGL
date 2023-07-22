#include <stdio.h>
#include "windowview.hpp"

WindowView::WindowView() {
    frameController = nullptr;
    printf("Initializing window\n");
}

void WindowView::onStart() {
    printf("Starting loop\n");
}

void WindowView::mainLoop() {
    printf("Updating window - Iteration %d\n", counter);
    counter++;
    if (counter > 10) {
        if (frameController != nullptr) {
            frameController->stop();
        }
    }
}

void WindowView::onStop() {
    printf("Stopping loop\n");
}
