#include "StopAfterTime.hpp"

namespace basil {

StopAfterTime::StopAfterTime() : IBasilWidget({
    "StopAfterTime",
    ProcessOrdinal::LATE,
    ProcessPrivilege::HIGH,
    WidgetPubSubPrefs::NONE
}) {}

void StopAfterTime::setFramesUntilStop(unsigned int numberOfFrames) {
    framesToRun = numberOfFrames;
}

void StopAfterTime::setTimeUntilStop(float timeUntilStopInSeconds) {
    timeToRun = std::chrono::milliseconds(
        static_cast<int>(timeUntilStopInSeconds * 1'000.f));
}

void StopAfterTime::onStart() {
    auto startTime = FrameClock::now();
    stopTime = startTime + timeToRun;

    stopFrame = currentFrame + framesToRun;
}

void StopAfterTime::onLoop() {
    auto currentTime = FrameClock::now();

    if (currentFrame >= stopFrame || currentTime >= stopTime) {
        setCurrentState(ProcessState::REQUEST_STOP);
    }

    currentFrame++;
}

StopAfterTime::Builder&
StopAfterTime::Builder::setTimeUntilStop(float timeUntilStopInSeconds) {
    this->impl->setTimeUntilStop(timeUntilStopInSeconds);
    return (*this);
}

StopAfterTime::Builder&
StopAfterTime::Builder::setFramesUntilStop(unsigned int numberOfFrames) {
    this->impl->setFramesUntilStop(numberOfFrames);
    return (*this);
}


}  // namespace basil
