#include "SystemTimeWatcher.hpp"

namespace basil {

SystemTimeWatcher::SystemTimeWatcher() : IBasilWidget(
    ProcessOrdinal::EARLY,
    ProcessPrivilege::NONE,
    "SystemTimeWatcher") {}

void SystemTimeWatcher::onStart() {
    startTime = FrameClock::now();
    lastFrameTime = startTime;

    model.frameNumber = 0;
    model.timeSinceFrame = FrameClock::duration::zero();
    model.timeSinceStart = FrameClock::duration::zero();
}

void SystemTimeWatcher::onLoop() {
    FrameClock::time_point currentTime = FrameClock::now();

    model.frameNumber++;
    model.timeSinceFrame = currentTime - lastFrameTime;
    model.timeSinceStart = currentTime - startTime;

    lastFrameTime = currentTime;

    this->IDataPublisher::publishData(DataMessage(model));
}

}  // namespace basil
