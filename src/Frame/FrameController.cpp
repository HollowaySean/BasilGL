#include <functional>
#include <list>
#include <string>
#include <utility>

#include "FrameController.hpp"
#include "DefaultTimerSource.hpp"

FrameController::FrameController(
    IFrameProcess *runnable,
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

void FrameController::addRunnable(
        IFrameProcess *newRunnable,
        Order orderToInsert) {
    switch (orderToInsert) {
        case Order::FIRST:
            mainRunnables.push_front(newRunnable);
            break;
        case Order::LAST:
        default:
            mainRunnables.push_back(newRunnable);
            break;
    }
}

void FrameController::removeRunnable(IFrameProcess *runnableToRemove) {
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
        timerSource->setMinimumFrameTime(frameTime);
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
    // TODO(sholloway): Break out function/class for IFrameProcess collection
    // TODO(sholloway): Add null checks before each run
    std::list<IFrameProcess*>::iterator iter;
    for (iter = mainRunnables.begin(); iter != mainRunnables.end(); ++iter) {
        (*iter)->onStart();
    }

    while (shouldRunLoop()) {
        timerSource->frameStart();

        for (   iter = mainRunnables.begin();
                iter != mainRunnables.end();
                ++iter) {
            timerSource->processStart(-1);
            (*iter)->onLoop();
            timerSource->processDone(-1);

            if (shouldKillLoop()) {
                break;
            }
        }

        timerSource->frameDone();
        timerSource->waitForFrameTime();
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
    std::list<IFrameProcess*>::iterator iter;

    for (iter = mainRunnables.begin(); iter != mainRunnables.end(); iter++) {
        IFrameProcess *runnable = (*iter);

        // Null check
        if (!runnable) {
            removeRunnable(runnable);

            // Some sort of record? Or message?

            continue;
        }

        // Pre-run state check
        checkRunnableState(runnable);
        switch (currentState) {
            case IFrameProcess::State::REQUEST_KILL:
                return;
            case IFrameProcess::State::SKIP:
                currentState = IFrameProcess::State::READY;
                continue;
            default:
        }

        // Run
        runnable->onLoop();

        // Post-run state check
        checkRunnableState(runnable);
        switch (currentState) {
            case IFrameProcess::State::REQUEST_STOP:
                // Take timestamp
            case IFrameProcess::State::REQUEST_KILL:
                return;
            default:
                currentState = IFrameProcess::State::READY;
        }

        // Take timestamp

    }
}

void FrameController::checkRunnableState(
    IFrameProcess *runnable) {
        IFrameProcess::State state = runnable->getCurrentState();
        if (state > currentState) {
            currentState = state;
        }
}
*/
