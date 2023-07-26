#include <functional>
#include <list>
#include <string>
#include <utility>

#include "FrameController.hpp"
#include "DefaultTimerSource.hpp"

FrameController::FrameController(
    IRunnable *runnable,
    ITimerSource *timerSource): timerSource(), frameMetrics(), currentState() {

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

void FrameController::addRunnable(IRunnable *newRunnable, Order orderToInsert) {
    switch (orderToInsert) {
        case Order::FIRST:
            mainRunnables.push_front(newRunnable);
            break;
        case Order::LAST:
        default:
            mainRunnables.push_back(newRunnable);
            break;
    }

    // Clear metrics to avoid runtime list confusion
    frameMetrics.clear();
}

void FrameController::removeRunnable(IRunnable *runnableToRemove) {
    // Clear metrics to avoid runtime list confusion
    frameMetrics.clear();

    if (!runnableToRemove) {
        return;
    }

    // TODO(sholloway): Correct segmentation fault here
    mainRunnables.remove(runnableToRemove);
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
    // TODO(sholloway): Break out function/class for IRunnable collection
    // TODO(sholloway): Add null checks before each run
    std::list<IRunnable*>::iterator iter;
    for (iter = mainRunnables.begin(); iter != mainRunnables.end(); ++iter) {
        (*iter)->onStart();
    }

    while (shouldRunLoop()) {
        float startTime = timerSource->startTimer();

        FrameMetrics::ReportList timeReports;

        for (iter = mainRunnables.begin();
            iter != mainRunnables.end();
            ++iter) {
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

    for (iter = mainRunnables.begin(); iter != mainRunnables.end(); ++iter) {
        (*iter)->onStop();
    }
}

bool FrameController::shouldStartLoop() {
    return currentState == FrameControllerState::STOPPED
        && !mainRunnables.empty();
}

bool FrameController::shouldRunLoop() {
    return currentState == FrameControllerState::RUNNING;
}

bool FrameController::shouldKillLoop() {
    return currentState == FrameControllerState::KILLED;
}

/**
void FrameController::safelyRunCollection(
        std::function<void()> method) {
    std::list<IRunnable*>::iterator iter;

    for (iter = mainRunnables.begin(); iter != mainRunnables.end(); iter++) {
        IRunnable *runnable = (*iter);

        // Null check
        if (!runnable) {
            removeRunnable(runnable);

            // Some sort of record? Or message?

            continue;
        }

        // Pre-run state check
        checkRunnableState(runnable);
        switch (currentState) {
            case IRunnable::State::REQUEST_KILL:
                return;
            case IRunnable::State::SKIP:
                currentState = IRunnable::State::READY;
                continue;
            default:
        }

        // Run
        runnable->mainLoop();

        // Post-run state check
        checkRunnableState(runnable);
        switch (currentState) {
            case IRunnable::State::REQUEST_STOP:
                // Take timestamp
            case IRunnable::State::REQUEST_KILL:
                return;
            default:
                currentState = IRunnable::State::READY;
        }

        // Take timestamp

    }
}

void FrameController::checkRunnableState(
    IRunnable *runnable) {
        IRunnable::State state = runnable->getCurrentState();
        if (state > currentState) {
            currentState = state;
        }
}
*/
