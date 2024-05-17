#include <chrono>

#include <catch.hpp>

#include "Chrono/TimeSource.hpp"
#include "ChronoTestUtils.hpp"

using basil::TimeSource;

using TSTest = TimeSource<TestClock>;
using TSChrono = TimeSource<std::chrono::steady_clock>;

TEST_CASE("Process_TimeSource_getTimestamp") {
    SECTION("Returns value specified by clock source") {
        TestClock::setNextTimeStamp(0);
        CHECK(TSTest::getTimestamp() ==
            TestClock::time_point(TestClock::duration(0)));

        TestClock::setNextTimeStamp(1);
        CHECK(TSTest::getTimestamp() ==
            TestClock::time_point(TestClock::duration(1)));
    }
}

TEST_CASE("Process_TimeSource_waitForDuration") {
    SECTION("Sleeps for time provided") {
        auto sleepTime = std::chrono::milliseconds(100);

        auto timeBeforeSleep = TSChrono::getTimestamp();
        TSChrono::waitForDuration(sleepTime);
        auto timeAfterSleep = TSChrono::getTimestamp();

        CHECK(timeAfterSleep - timeBeforeSleep >= sleepTime);
    }
}

TEST_CASE("Process_TimeSource_waitUntilTime") {
    SECTION("Sleeps for time provided") {
        auto sleepTime = std::chrono::milliseconds(100);

        auto timeBeforeSleep = TSChrono::getTimestamp();
        TSChrono::waitUntilTime(timeBeforeSleep + sleepTime);
        auto timeAfterSleep = TSChrono::getTimestamp();

        CHECK(timeAfterSleep - timeBeforeSleep >= sleepTime);
    }
}

TEST_CASE("Process_TimeSource_frequencyToPeriod") {
    SECTION("Returns zero for zero input") {
        auto period = TSTest::frequencyToPeriod(0);
        CHECK(period == std::chrono::seconds(0));
    }

    SECTION("Converts frequency to time") {
        auto period = TSTest::frequencyToPeriod(100);
        CHECK(period == std::chrono::milliseconds(10));
    }
}

TEST_CASE("Process_TimeSource_periodToFrequency") {
    SECTION("Returns zero for zero input") {
        auto period = std::chrono::milliseconds(0);
        double frequency = TSTest::periodToFrequency(period);

        CHECK(frequency == 0.);
    }

    SECTION("Converts time to frequency") {
        auto period = std::chrono::milliseconds(10);
        double frequency = TSTest::periodToFrequency(period);

        CHECK(frequency == 100.);
    }
}
