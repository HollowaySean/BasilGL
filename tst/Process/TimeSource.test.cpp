#include <chrono>

#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ChronoTestUtils.hpp"

using basil::TimeSource;

TEST_CASE("Process_TimeSource_getTimestamp") {
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

TEST_CASE("Process_TimeSource_waitForDuration") {
    SECTION("Sleeps for time provided") {
        auto timeSource = TimeSource<std::chrono::steady_clock>();

        auto sleepTime = std::chrono::milliseconds(100);

        auto timeBeforeSleep = timeSource.getTimestamp();
        timeSource.waitForDuration(sleepTime);
        auto timeAfterSleep = timeSource.getTimestamp();

        REQUIRE(timeAfterSleep - timeBeforeSleep >= sleepTime);
    }
}

TEST_CASE("Process_TimeSource_waitUntilTime") {
    SECTION("Sleeps for time provided") {
        auto timeSource = TimeSource<std::chrono::steady_clock>();

        auto sleepTime = std::chrono::milliseconds(100);

        auto timeBeforeSleep = timeSource.getTimestamp();
        timeSource.waitUntilTime(timeBeforeSleep + sleepTime);
        auto timeAfterSleep = timeSource.getTimestamp();

        REQUIRE(timeAfterSleep - timeBeforeSleep >= sleepTime);
    }
}

TEST_CASE("Process_TimeSource_frequencyToPeriod") {
    auto timeSource = TimeSource<std::chrono::steady_clock>();

    SECTION("Returns zero for zero input") {
        auto period = timeSource.frequencyToPeriod(0);
        REQUIRE(period == std::chrono::seconds(0));
    }

    SECTION("Converts frequency to time") {
        auto period = timeSource.frequencyToPeriod(100);
        REQUIRE(period == std::chrono::milliseconds(10));
    }
}
