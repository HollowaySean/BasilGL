#include "MetricsRecord.hpp"

namespace basil {

MetricsRecord MetricsRecord::operator+(MetricsRecord addend) {
    this->frameTime += addend.frameTime;
    this->workTime += addend.workTime;

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

MetricsRecord MetricsRecord::operator-(MetricsRecord addend) {
    this->frameTime -= addend.frameTime;
    this->workTime -= addend.workTime;

    for (auto process : processTimes) {
        this->processTimes[process.first] -=
            addend.processTimes.contains(process.first)
                ? addend.processTimes[process.first]
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

}  // namespace basil
