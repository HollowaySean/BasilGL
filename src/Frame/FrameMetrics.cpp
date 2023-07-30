#include "FrameMetrics.hpp"


FrameMetricsRecord FrameMetricsRecord::operator+(FrameMetricsRecord other) {
    FrameMetricsRecord result = FrameMetricsRecord();
    result.frameTime = frameTime + other.frameTime;
    result.workTime = workTime + other.workTime;

    for (auto process : processTime) {
        int processID = process.first;
        double processTime = process.second;

        processTime += other.processTime.count(processID)
            ? other.processTime[processID]
            : 0;

        result.processTime.emplace(processID, processTime);
    }

    // Loop over other to find any processes missing from the other summand
    for (auto process : other.processTime) {
        if (!processTime.count(process.first)) {
            result.processTime.emplace(process);
        }
    }

    return result;
}

FrameMetricsRecord FrameMetricsRecord::operator-(FrameMetricsRecord other) {
    FrameMetricsRecord result = FrameMetricsRecord();
    result.frameTime = frameTime - other.frameTime;
    result.workTime = workTime - other.workTime;

    for (auto process : processTime) {
        int frameID = process.first;
        double processTime = process.second;

        processTime -= other.processTime.count(frameID)
            ? other.processTime[frameID]
            : 0;

        result.processTime.emplace(frameID, processTime);
    }

    return result;
}

FrameMetricsRecord FrameMetricsRecord::operator/(int divisor) {
    FrameMetricsRecord result = FrameMetricsRecord();
    result.frameTime = frameTime / divisor;
    result.workTime = workTime / divisor;

    for (auto process : processTime) {
        int frameID = process.first;
        result.processTime[frameID] = process.second / divisor;
    }

    return result;
}

double FrameMetricsRecord::getFrameRate() {
    double frameTimeInSeconds = frameTime / 1000.;
    return 1. / frameTimeInSeconds;
}

double FrameMetricsRecord::getUncappedFrameRate() {
    double workTimeInSeconds = workTime / 1000.;
    return 1. / workTimeInSeconds;
}

FrameMetrics::FrameMetrics(int newBufferSize) {
    clearBuffer();
    setBufferSize(newBufferSize);
}

void FrameMetrics::setBufferSize(int newBufferSize) {
    if (newBufferSize < bufferSize) {
        clearBuffer();
    }
    bufferSize = newBufferSize;
}

int FrameMetrics::getBufferSize() {
    return bufferSize;
}

int FrameMetrics::getBufferCount() {
    return buffer.size();
}

void FrameMetrics::pushTimerRecord(TimerRecord record) {
    FrameMetricsRecord newMetricsRecord =
        FrameMetricsRecord::fromTimestamp(record);

    currentSum = currentSum + newMetricsRecord;
    buffer.push_front(newMetricsRecord);

    if (buffer.size() > bufferSize) {
        FrameMetricsRecord recordToRemove = buffer.back();
        currentSum = currentSum - recordToRemove;
        buffer.pop_back();
    }
}

FrameMetricsRecord FrameMetrics::getCurrentMetrics() {
    return currentSum / getBufferCount();
}

void FrameMetrics::clearBuffer() {
    currentSum = FrameMetricsRecord();
    buffer.clear();
}

FrameMetricsRecord FrameMetricsRecord::fromTimestamp(TimerRecord record) {
    FrameMetricsRecord newMetrics = FrameMetricsRecord(record.frameID);

    newMetrics.workTime = record.frameDone - record.frameStart;
    newMetrics.frameTime = record.frameEnd - record.frameStart;

    for (auto process : record.processStart) {
        int processID = process.first;
        double processStart = process.second;

        if (!record.processDone.count(processID)) {
            continue;
        }

        double processDone = record.processDone[processID];
        double processTime = processDone - processStart;

        newMetrics.processTime.emplace(processID, processTime);
    }

    return newMetrics;
}
