#pragma once

#include <memory>
#include <queue>

#include <Basil/Packages/Chrono.hpp>

#include "MetricsRecord.hpp"

#ifdef TEST_BUILD
#include "Chrono/ChronoTestUtils.hpp"
#endif

namespace basil {

class MetricsObserver {
 public:
    /** @brief Record timestamp of frame start. */
    void recordFrameStart(FrameClock::time_point frameStartTime);

    /** @brief Record the run time of an individual process. */
    void recordProcessTime(std::shared_ptr<ProcessInstance> instance,
        FrameClock::duration duration);

    /** @brief Record the time taken to run each process. */
    void recordWorkEnd(
        FrameClock::time_point workEndTime);

    /** @brief Record the time taken for entire frame. */
    void recordFrameEnd(
        FrameClock::time_point frameEndTime);

    /** @return Current averaged metrics. */
    MetricsRecord getCurrentMetrics();

    /** @return Most recent frame's metrics. */
    MetricsRecord getLatestMetrics();

    /** @param newBufferSize Number of frames to average over. */
    void setBufferSize(unsigned int newBufferSize);

    /** @return Size of averaging window. */
    unsigned int getBufferSize() { return bufferSize; }

    /** @return Number of frame records currently in buffer. */
    unsigned int getBufferCount();

#ifndef TEST_BUILD

 private:
#endif
    unsigned int currentFrame = 0;
    unsigned int bufferSize = 1;

    std::queue<MetricsRecord> buffer;
    MetricsRecord sum;
    MetricsRecord current;

    FrameClock::time_point frameStartTime;

    void pushFrameToBuffer();
    void popFrameFromBuffer();
};

}   // namespace basil
