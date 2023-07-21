#include "framecontroller.hpp"

#include <thread>

FrameController::FrameController(Runnable *runnable = nullptr) {
    setRunnable(runnable);
    setFrameCap(0);
    currentState = FrameControllerState::STOPPED;
}

void FrameController::setFrameCap(int framesPerSecond) {
    frameCap = framesPerSecond;
    if (frameCap > 0) {
        frameTime = std::chrono::milliseconds(1000) / frameCap;
    }
}

void FrameController::start() {
    if (!shouldStartLoop()) {
        return;
    }

    currentState = FrameControllerState::RUNNING;
    runLoop();
}

void FrameController::stop() {
    currentState = FrameControllerState::STOPPING;
}

void FrameController::runLoop() {
    while (shouldRunLoop()) {
        loopStart = std::chrono::steady_clock::now();
        runnable->mainLoop();
        loopEnd = std::chrono::steady_clock::now();
        loopTime = loopEnd - loopStart;

        waitForFrameEnd();
    }

    currentState = FrameControllerState::STOPPED;
}

void FrameController::waitForFrameEnd() {
    if (frameCap <= 0) {
        return;
    }

    std::chrono::steady_clock::duration waitTime = frameTime - loopTime;
    if (waitTime > std::chrono::milliseconds(0)) {
        std::this_thread::sleep_for(waitTime);
    }
}

bool FrameController::shouldStartLoop() {
    return currentState == FrameControllerState::STOPPED
        && runnable != nullptr;
}

bool FrameController::shouldRunLoop() {
    return currentState == FrameControllerState::RUNNING;
}