#include <chrono>

#include <catch.hpp>

#include <Basil/Frame.hpp>

#include "ChronoTestUtils.hpp"

using basil::TimeSource;

TEST_CASE("Frame_TimeSource_now") {
    SECTION("Returns value specified by clock source") {
        TimeSource<TestClock> timeSource = TimeSource<TestClock>();

        REQUIRE(timeSource.getTimestamp() ==
            TestClock::time_point(TestClock::duration(0)));
        TestClock::nextTimeStamp = 1;
        REQUIRE(timeSource.getTimestamp() ==
            TestClock::time_point(TestClock::duration(1)));
        TestClock::nextTimeStamp = 0;
    }
}
