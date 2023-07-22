#include <stdio.h>
#include <thread>
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
    if (counter > 20) {
        frameController->stop();
    }

    FrameMetrics::MetricsReport report =
        (frameController->frameMetrics).getMetricsReport();
    printf("Frame rate is %.2f FPS\n", report.frameRate);
    printf("Frame time is %.2f ms\n", report.frameTimeMS);
    printf("Work rate is %.2f FPS\n", report.uncappedFrameRate);
    printf("Work time is %.2f ms\n", report.workTimeMS);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void WindowView::onStop() {
    printf("Stopping loop\n");
}
