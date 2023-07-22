#include "framecontroller.hpp"

void DefaultTimerSource::setMinimumWaitTime(float waitTimeInMS) {
    // TODO(sholloway): Fix static cast
    minimumWaitTime = std::chrono::milliseconds(static_cast<int>(waitTimeInMS));
}

float DefaultTimerSource::startTimer() {
    timerStart = std::chrono::steady_clock::now();

    std::chrono::duration<float> timerStartSeconds
        = timerStart.time_since_epoch();
    return 1000.0 * timerStartSeconds.count();
}

float DefaultTimerSource::stopTimer() {
    timerStop = std::chrono::steady_clock::now();
    elapsedTime = timerStop - timerStart;


    std::chrono::duration<float> timerStopSeconds
        = timerStop.time_since_epoch();
    return 1000.0 * timerStopSeconds.count();
}

float DefaultTimerSource::waitForTime() {
    std::chrono::duration<float> timeToWait
        = minimumWaitTime - elapsedTime;

    std::this_thread::sleep_for(timeToWait);

    return 1000.0 * timeToWait.count();
}
