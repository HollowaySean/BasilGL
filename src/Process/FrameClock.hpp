#ifndef SRC_PROCESS_FRAMECLOCK_HPP_
#define SRC_PROCESS_FRAMECLOCK_HPP_

#include <chrono>
#include "TimeSource.hpp"

// Include stubbed clock source for test build
#ifdef TEST_BUILD
    #include "Process/ChronoTestUtils.hpp"

namespace basil {
    using FrameClock = TestClock;

#else

namespace basil {
    using FrameClock = std::chrono::steady_clock;

#endif

using FrameTimer = TimeSource<FrameClock>;

}  // namespace basil

#endif  // SRC_PROCESS_FRAMECLOCK_HPP_
