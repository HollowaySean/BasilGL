#include "framecontroller.hpp"

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
