#include "MetricsObserver.hpp"

namespace basil {

void MetricsObserver::recordFrameStart(FrameClock::time_point frameStartTime) {
    this->current = MetricsRecord(currentFrame++);
    this->frameStartTime = frameStartTime;
}

void MetricsObserver::recordProcessTime(
        std::shared_ptr<ProcessInstance> instance,
        FrameClock::duration duration) {
    current.processTimes.emplace(instance, duration);
}

void MetricsObserver::recordWorkEnd(FrameClock::time_point workEndTime) {
    current.workTime = workEndTime - frameStartTime;
}

void MetricsObserver::recordFrameEnd(FrameClock::time_point frameEndTime) {
    current.frameTime = frameEndTime - frameStartTime;
    pushFrameToBuffer();
}

MetricsRecord MetricsObserver::getCurrentMetrics() {
    return sum / getBufferCount();
}

MetricsRecord MetricsObserver::getLatestMetrics() {
    return buffer.back();
}

void MetricsObserver::setBufferSize(unsigned int newBufferSize) {
    if (newBufferSize == 0) return;

    bufferSize = newBufferSize;

    while (buffer.size() > bufferSize) {
        popFrameFromBuffer();
    }
}

unsigned int MetricsObserver::getBufferCount() {
    return buffer.size();
}

void MetricsObserver::pushFrameToBuffer() {
    sum = sum + current;
    buffer.emplace(current);

    if (getBufferCount() > bufferSize) {
        popFrameFromBuffer();
    }
}

void MetricsObserver::popFrameFromBuffer() {
    MetricsRecord oldest = buffer.front();

    sum = sum - oldest;
    buffer.pop();
}

}  // namespace basil
