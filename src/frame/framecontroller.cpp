#include "framecontroller.hpp"

FrameController::FrameController(
    Runnable *runnable,
    TimerSource *timerSource): timerSource() {
    setRunnable(runnable);

    if (!timerSource) {
        timerSource = new DefaultTimerSource();
    }
    setTimerSource(timerSource);
    setFrameCap(0);

    currentState = FrameControllerState::STOPPED;
}

void FrameController::setFrameCap(int framesPerSecond) {
    frameCap = framesPerSecond;

    if (frameCap > 0) {
        int frameTime = (1000.0 / frameCap);
        timerSource->setMinimumWaitTime(frameTime);
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
    runnable->onStart();

    while (shouldRunLoop()) {
        timerSource->startTimer();
        runnable->mainLoop();
        timerSource->stopTimer();

        timerSource->waitForTime();
    }

    currentState = FrameControllerState::STOPPED;
    runnable->onStop();
}

bool FrameController::shouldStartLoop() {
    return currentState == FrameControllerState::STOPPED
        && runnable != nullptr;
}

bool FrameController::shouldRunLoop() {
    return currentState == FrameControllerState::RUNNING;
}
