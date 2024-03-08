#include "MetricsRecord.hpp"

namespace basil {

MetricsRecord MetricsRecord::operator+(MetricsRecord addend) {
    this->frameTime += addend.frameTime;
    this->workTime += addend.workTime;

    if (addend.frameID > this->frameID) {
        this->frameID = addend.frameID;
    }

    for (auto process : processTimes) {
        this->processTimes[process.first] +=
            addend.processTimes.contains(process.first)
                ? addend.processTimes[process.first]
                : FrameClock::duration::zero();
    }

    for (auto process : addend.processTimes) {
        if (!this->processTimes.contains(process.first)) {
            this->processTimes.emplace(process);
        }
    }

    return *this;
}

MetricsRecord MetricsRecord::operator-(MetricsRecord subtrahend) {
    this->frameTime -= subtrahend.frameTime;
    this->workTime -= subtrahend.workTime;

    if (subtrahend.frameID > this->frameID) {
        this->frameID = subtrahend.frameID;
    }

    for (auto process : processTimes) {
        this->processTimes[process.first] -=
            subtrahend.processTimes.contains(process.first)
                ? subtrahend.processTimes[process.first]
                : FrameClock::duration::zero();
    }

    return *this;
}

MetricsRecord MetricsRecord::operator/(int divisor) {
    this->frameTime /= divisor;
    this->workTime /= divisor;

    for (auto process : processTimes) {
        this->processTimes[process.first] = process.second / divisor;
    }

    return *this;
}

bool MetricsRecord::isEqual(const MetricsRecord& comparison) {
    bool samePrimitives =
            frameID      == comparison.frameID
        &&  frameTime    == comparison.frameTime
        &&  workTime     == comparison.workTime;

    bool sameMap =
        processTimes.size() == comparison.processTimes.size()
            && std::equal(processTimes.begin(), processTimes.end(),
                          comparison.processTimes.begin());

    return samePrimitives && sameMap;
}

double MetricsRecord::getFrameRate() {
    return Timer::periodToFrequency(frameTime);
}

double MetricsRecord::getUncappedFrameRate() {
    return Timer::periodToFrequency(workTime);
}

}  // namespace basil
