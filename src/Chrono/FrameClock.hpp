#pragma once

#include <chrono>
#include "TimeSource.hpp"

// Include stubbed clock source for test build
#ifdef TEST_BUILD
    #include "Chrono/ChronoTestUtils.hpp"

namespace basil {
    using FrameClock = TestClock;

#else

namespace basil {
    using FrameClock = std::chrono::steady_clock;

#endif

using FrameTimer = TimeSource<FrameClock>;

}   // namespace basil
