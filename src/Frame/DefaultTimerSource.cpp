#include "DefaultTimerSource.hpp"

void DefaultTimerSource::frameStart() {
    currentRecord = TimerRecord(++frameID);
    currentRecord.frameStart = readTimer();
}

void DefaultTimerSource::frameDone() {
    currentRecord.frameDone = readTimer();
}

void DefaultTimerSource::waitForFrameTime() {
    double elapsedTimeInMS =
        currentRecord.frameDone - currentRecord.frameStart;
    double elapsedTimeInSeconds = elapsedTimeInMS / 1000.;
    double timeToWaitInSeconds = waitTimeInSeconds - elapsedTimeInSeconds;

    if (timeToWaitInSeconds > 0) {
        auto timeToWaitInMS = secondsToDuration(timeToWaitInSeconds);
        std::this_thread::sleep_for(timeToWaitInMS);
    }

    currentRecord.frameEnd = readTimer();
}

void DefaultTimerSource::processStart(int processID) {
    double startTime = readTimer();
    currentRecord.processStart.emplace(processID, startTime);
}

void DefaultTimerSource::processDone(int processID) {
    double stopTime = readTimer();
    currentRecord.processDone.emplace(processID, stopTime);
}

double DefaultTimerSource::readTimer() {
    time_point timerNow = steady_clock::now();
    return timePointToMilliseconds(timerNow);
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
