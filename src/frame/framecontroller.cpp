#include "framecontroller.hpp"

#include <thread>

void DefaultTimerSource::setMinimumWaitTime(int waitTimeInMS) {
    minimumWaitTime = std::chrono::milliseconds(waitTimeInMS);
}

int DefaultTimerSource::startTimer() {
    timerStart = std::chrono::steady_clock::now();

    std::chrono::milliseconds timerStartMS =
        std::chrono::duration_cast<std::chrono::milliseconds>
            (timerStart.time_since_epoch());
    return timerStartMS.count();
}

int DefaultTimerSource::stopTimer() {
    timerStop = std::chrono::steady_clock::now();
    elapsedTime = timerStop - timerStart;

    std::chrono::milliseconds timerStopMS =
        std::chrono::duration_cast<std::chrono::milliseconds>
            (timerStart.time_since_epoch());
    return timerStopMS.count();
}

int DefaultTimerSource::waitForTime() {
    std::chrono::steady_clock::duration timeToWait
        = minimumWaitTime - elapsedTime;

    std::this_thread::sleep_for(timeToWait);

    std::chrono::milliseconds waitTimeMS =
        std::chrono::duration_cast<std::chrono::milliseconds>
            (timeToWait);
    return waitTimeMS.count();
}


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
    while (shouldRunLoop()) {
        timerSource->startTimer();
        runnable->mainLoop();
        timerSource->stopTimer();

        timerSource->waitForTime();
    }

    currentState = FrameControllerState::STOPPED;
}

bool FrameController::shouldStartLoop() {
    return currentState == FrameControllerState::STOPPED
        && runnable != nullptr;
}

bool FrameController::shouldRunLoop() {
    return currentState == FrameControllerState::RUNNING;
}
