#include "DefaultTimerSource.hpp"

void DefaultTimerSource::setMinimumWaitTime(double waitTime) {
    minimumWaitTime = secondsToDuration(waitTime);
}

double DefaultTimerSource::startTimer() {
    timerStart = steady_clock::now();
    return timePointToMilliseconds(timerStart);
}

double DefaultTimerSource::readTimer() {
    time_point timerNow = steady_clock::now();
    return timePointToMilliseconds(timerNow);
}

double DefaultTimerSource::stopTimer() {
    timerStop = steady_clock::now();
    return timePointToMilliseconds(timerStop);
}

double DefaultTimerSource::waitForTime() {
    duration timeToWait = minimumWaitTime - elapsedTime();

    std::this_thread::sleep_for(timeToWait);

    double timeToWaitMS = durationToMilliseconds(timeToWait);

    return timeToWaitMS > 0. ? timeToWaitMS : 0.;
}

DefaultTimerSource::duration
DefaultTimerSource::secondsToDuration(double timeInSeconds) {
    return std::chrono::duration<double, std::ratio<1>>(timeInSeconds);
}

double DefaultTimerSource::timePointToMilliseconds(time_point timePoint) {
    auto timePointMS =
        std::chrono::time_point_cast<std::chrono::milliseconds>(timePoint);
    return timePointMS.time_since_epoch().count();
}

double DefaultTimerSource::durationToMilliseconds(
        DefaultTimerSource::duration duration) {
    auto durationMS =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    return durationMS.count();
}
