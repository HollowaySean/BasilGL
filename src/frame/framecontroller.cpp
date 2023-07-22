#include "framecontroller.hpp"

FrameController::FrameController(
    Runnable *runnable,
    TimerSource *timerSource): timerSource(), frameMetrics() {

    if (runnable) {
        addRunnable(runnable);
    }

    if (!timerSource) {
        timerSource = new DefaultTimerSource();
    }
    setTimerSource(timerSource);
    setFrameCap(0);

    currentState = FrameControllerState::STOPPED;
}

void FrameController::addRunnable(Runnable *newRunnable) {
    newRunnable->setController(this);
    runnables.insert(newRunnable);
}

void FrameController::removeRunnable(Runnable *runnableToRemove) {
    runnableToRemove->setController(nullptr);
    runnables.erase(runnableToRemove);
}

void FrameController::setFrameCap(int framesPerSecond) {
    frameCap = framesPerSecond;

    if (frameCap > 0) {
        float frameTime = (1000.0 / frameCap);
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
    std::set<Runnable*>::iterator iter;
    for (iter = runnables.begin(); iter != runnables.end(); ++iter) {
        (*iter)->onStart();
    }

    while (shouldRunLoop()) {
        float startTime = timerSource->startTimer();

        for (iter = runnables.begin(); iter != runnables.end(); ++iter) {
            (*iter)->mainLoop();
        }

        float stopTime = timerSource->stopTimer();
        float waitTime = timerSource->waitForTime();

        frameMetrics.updateTimeStamps(startTime, stopTime, waitTime);
    }

    currentState = FrameControllerState::STOPPED;

    for (iter = runnables.begin(); iter != runnables.end(); ++iter) {
        (*iter)->onStop();
    }
}

bool FrameController::shouldStartLoop() {
    return currentState == FrameControllerState::STOPPED
        && !runnables.empty();
}

bool FrameController::shouldRunLoop() {
    return currentState == FrameControllerState::RUNNING;
}
