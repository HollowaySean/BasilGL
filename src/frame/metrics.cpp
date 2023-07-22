#include "framecontroller.hpp"

FrameMetrics::FrameMetrics(int setWindowSize) {
    windowSize = setWindowSize;
}

void FrameMetrics::updateTimeStamps(float start, float stop, float wait) {
    TimeStamp newTimestamp = { start, stop, wait };

    sumFrameTime += newTimestamp.frame();
    sumWorkTime += newTimestamp.work();

    int numberOfTimestamps = timestamps.size();
    if (numberOfTimestamps == windowSize) {
        FrameMetrics::TimeStamp timestampToRemove = timestamps.back();
        sumFrameTime -= timestampToRemove.frame();
        sumWorkTime -= timestampToRemove.work();

        timestamps.pop_back();
    }

    timestamps.push_front(newTimestamp);
}

void FrameMetrics::setSmoothingWindow(int newWindowSize) {
    windowSize = newWindowSize;
}

float FrameMetrics::getFrameTimeMS() {
    return timestamps.size() == 0 ? -1.0 : sumFrameTime / timestamps.size();
}

float FrameMetrics::getFrameRate() {
    return 1000.0 / (static_cast<float>(getFrameTimeMS()));
}

float FrameMetrics::getWorkTimeMS() {
    return timestamps.size() == 0 ? -1.0 : sumWorkTime / timestamps.size();
}

float FrameMetrics::getUncappedFrameRate() {
    return 1000.0 / (static_cast<float>(getWorkTimeMS()));
}

FrameMetrics::MetricsReport FrameMetrics::getMetricsReport() {
    return {
        getFrameTimeMS(),
        getWorkTimeMS(),
        getFrameRate(),
        getUncappedFrameRate(),
        windowSize
    };
}
