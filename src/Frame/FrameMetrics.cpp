#include <algorithm>

#include "FrameMetrics.hpp"

FrameMetrics::FrameMetrics(int setWindowSize) {
    windowSize = setWindowSize;
}

void FrameMetrics::updateTimeStamps(
        float start, float stop, float wait,
        ReportList doneTimes
    ) {
    // Transform from timestamps into durations
    ReportList runTimes;
    std::transform(doneTimes.begin(), doneTimes.end(),
        std::back_inserter(runTimes),
        [start](TimeReport endReport){
            return TimeReport(endReport.first - start, endReport.second);
        });

    FrameTimeStamps newTimestamp = { start, stop, wait, doneTimes, runTimes };

    sumFrameTime += newTimestamp.frame();
    sumWorkTime += newTimestamp.work();
    if (sumRunTimes.size() == 0) {
        sumRunTimes = runTimes;
    } else {
        std::transform(runTimes.begin(), runTimes.end(), sumRunTimes.begin(),
            sumRunTimes.begin(), [](TimeReport newTime, TimeReport sumTime){
                return TimeReport(newTime.first + sumTime.first,
                    newTime.second);
            });
    }

    // Update moving averages
    int numberOfTimestamps = timestamps.size();
    if (numberOfTimestamps == windowSize) {
        FrameMetrics::FrameTimeStamps timestampToRemove = timestamps.back();
        sumFrameTime -= timestampToRemove.frame();
        sumWorkTime -= timestampToRemove.work();

        ReportList reportToRemove = timestampToRemove.doneTimes;
        std::transform(reportToRemove.begin(), reportToRemove.end(),
            sumRunTimes.begin(), sumRunTimes.begin(),
            [](TimeReport removeTime, TimeReport sumTime){
                return TimeReport(sumTime.first - removeTime.first,
                    sumTime.second);
            });

        timestamps.pop_back();
    }

    timestamps.push_front(newTimestamp);
}

void FrameMetrics::setSmoothingWindow(int newWindowSize) {
    if (newWindowSize < windowSize) {
        clear();
    }
    windowSize = newWindowSize;
}

float FrameMetrics::getFrameTimeMS() {
    return timestamps.size() == 0 ? -1.0 :
        sumFrameTime / timestamps.size();
}

float FrameMetrics::getFrameRate() {
    return timestamps.size() == 0 ? -1.0 :
        1000.0 / (static_cast<float>(getFrameTimeMS()));
}

float FrameMetrics::getWorkTimeMS() {
    return timestamps.size() == 0 ? -1.0 :
        sumWorkTime / timestamps.size();
}

float FrameMetrics::getUncappedFrameRate() {
    return timestamps.size() == 0 ? -1.0 :
        1000.0 / (static_cast<float>(getWorkTimeMS()));
}

FrameMetrics::ReportList FrameMetrics::getRunTimesMS() {
    int numberOfValues = timestamps.size();
    if (numberOfValues == 0) {
        return ReportList();
    }

    ReportList output;
    std::transform(sumRunTimes.begin(), sumRunTimes.end(),
        std::back_inserter(output),
        [numberOfValues](TimeReport sumRunTime){
            return TimeReport(sumRunTime.first / numberOfValues,
                sumRunTime.second);
        });
    return output;
}

FrameMetrics::MetricsReport FrameMetrics::getMetricsReport() {
    return {
        getFrameTimeMS(),
        getWorkTimeMS(),
        getFrameRate(),
        getUncappedFrameRate(),
        getRunTimesMS(),
        windowSize
    };
}

void FrameMetrics::clear() {
    timestamps.clear();
    sumFrameTime = 0;
    sumWorkTime = 0;
    sumRunTimes.clear();
}
