#ifndef SRC_PROCESS_METRICSOBSERVER_HPP_
#define SRC_PROCESS_METRICSOBSERVER_HPP_

#include <chrono>
#include <map>
#include <memory>

#include "ProcessInstance.hpp"

#ifdef TEST_BUILD

#include "Process/ChronoTestUtils.hpp"
using FrameClock = TestClock;

#else
    using FrameClock = std::chrono::steady_clock;
#endif

namespace basil {

class MetricsObserver {
 public:
    /** @brief Record timestamp of frame start. */
    void recordFrameStart(FrameClock::time_point frameStartTime);

    /** @brief Record the run time of an individual process. */
    void recordProcessTime(std::shared_ptr<ProcessInstance> instance,
        FrameClock::duration duration) {}

    /** @brief Record the time taken to run each process. */
    void recordWorkEnd(
        FrameClock::time_point workEndTime) {}

    /** @brief Record the time taken for entire frame. */
    void recordFrameEnd(
        FrameClock::time_point frameEndTime) {}

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
};

}  // namespace basil

#endif  // SRC_PROCESS_METRICSOBSERVER_HPP_
