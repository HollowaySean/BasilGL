#ifndef SRC_PROCESS_METRICSOBSERVER_HPP_
#define SRC_PROCESS_METRICSOBSERVER_HPP_

#include <chrono>
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
    // Stubbed for now
    void recordProcessTime(std::shared_ptr<ProcessInstance> instance,
        FrameClock::duration duration) {}

    // Stubbed for now
    void recordFrameTime(
        FrameClock::duration duration) {}
};

}  // namespace basil

#endif  // SRC_PROCESS_METRICSOBSERVER_HPP_
