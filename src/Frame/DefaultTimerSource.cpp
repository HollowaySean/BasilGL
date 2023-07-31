#include "DefaultTimerSource.hpp"

std::unique_ptr<ITimerSource> DefaultTimerSource::clone() const {
    return std::make_unique<DefaultTimerSource>(*this);
}

void DefaultTimerSource::frameStart() {
    frameID++;

    currentRecord = TimerRecord(frameID);
    currentRecord.frameStart = readTimer();
}

void DefaultTimerSource::frameDone() {
    currentRecord.frameDone = readTimer();

    // Sets frameEnd as well, in case wait is never called
    currentRecord.frameEnd = currentRecord.frameDone;
}

void DefaultTimerSource::waitForFrameTime() {
    double elapsedTimeInSeconds =
        currentRecord.frameDone - currentRecord.frameStart;
    double timeToWaitInSeconds = waitTimeInSeconds - elapsedTimeInSeconds;

    if (timeToWaitInSeconds > 0) {
        auto durationToWait = secondsToDuration(timeToWaitInSeconds);
        std::this_thread::sleep_for(durationToWait);
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

double DefaultTimerSource::readTimer() const {
    time_point timerNow = steady_clock::now();
    return timePointToSeconds(timerNow);
}

DefaultTimerSource::duration
DefaultTimerSource::secondsToDuration(double timeInSeconds) {
    return std::chrono::duration<double, std::ratio<1>>(timeInSeconds);
}

double DefaultTimerSource::timePointToSeconds(time_point timePoint) {
    auto timePointNS =
        std::chrono::time_point_cast<std::chrono::nanoseconds>(timePoint);
    double timeCountNS = timePointNS.time_since_epoch().count();
    return timeCountNS / SECONDS_PER_NANO;
}
