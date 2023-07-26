#include "DefaultTimerSource.hpp"

void DefaultTimerSource::setMinimumWaitTime(float waitTimeInMS) {
    minimumWaitTime = std::chrono::milliseconds(static_cast<int>(waitTimeInMS));
}

float DefaultTimerSource::startTimer() {
    timerStart = std::chrono::steady_clock::now();

    std::chrono::duration<float> timerStartSeconds
        = timerStart.time_since_epoch();
    return 1000.0 * timerStartSeconds.count();
}

float DefaultTimerSource::readTimer() {
    std::chrono::time_point timerNow = std::chrono::steady_clock::now();

    std::chrono::duration<float> timerNowSeconds
        = timerNow.time_since_epoch();
    return 1000.0 * timerNowSeconds.count();
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
