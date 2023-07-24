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

void FrameController::addRunnable(Runnable *newRunnable, Order orderToInsert) {
    newRunnable->setController(this);

    switch (orderToInsert) {
        case Order::FIRST:
            runnables.push_front(newRunnable);
            break;
        case Order::LAST:
        default:
            runnables.push_back(newRunnable);
            break;
    }

    // Clear metrics to avoid runtime list confusion
    frameMetrics.clear();
}

void FrameController::removeRunnable(Runnable *runnableToRemove) {
    runnableToRemove->setController(nullptr);
    runnables.remove(runnableToRemove);

    // Clear metrics to avoid runtime list confusion
    frameMetrics.clear();
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

void FrameController::kill() {
    currentState = FrameControllerState::KILLED;
}

void FrameController::runLoop() {
    std::list<Runnable*>::iterator iter;
    for (iter = runnables.begin(); iter != runnables.end(); ++iter) {
        (*iter)->onStart();
    }

    while (shouldRunLoop()) {
        float startTime = timerSource->startTimer();

        FrameMetrics::ReportList timeReports;

        for (iter = runnables.begin(); iter != runnables.end(); ++iter) {
            (*iter)->mainLoop();

            float doneTime = timerSource->readTimer();
            FrameMetrics::TimeReport doneTimeReport(
                doneTime, (*iter)->getName());
            timeReports.push_back(doneTimeReport);

            if (shouldKillLoop()) {
                break;
            }
        }

        float stopTime = timerSource->stopTimer();
        float waitTime = timerSource->waitForTime();

        frameMetrics.updateTimeStamps(
            startTime, stopTime, waitTime, timeReports);
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

bool FrameController::shouldKillLoop() {
    return currentState == FrameControllerState::KILLED;
}

Runnable::~Runnable() {
    if (frameController) {
        frameController->removeRunnable(this);
    }
}
