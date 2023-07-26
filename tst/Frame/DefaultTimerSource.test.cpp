#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "DefaultTimerSource.hpp"

using std::chrono::steady_clock;
using duration = std::chrono::duration<double>;
using time_point = steady_clock::time_point;

const double WAIT_TIME_MS = 50.;
const double TIME_MARGIN_MS = 5.;

void requireWithinMargin(double expected, double measured) {
    REQUIRE(expected <= measured + TIME_MARGIN_MS);
    REQUIRE(measured <= expected + TIME_MARGIN_MS);
}

double timePointToMilliseconds(time_point timePoint) {
    auto timePointMS =
        std::chrono::time_point_cast<std::chrono::milliseconds>(timePoint);
    return timePointMS.time_since_epoch().count();
}

double durationToMilliseconds(duration duration) {
    auto durationMS =
        std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    return durationMS.count();
}

void checkTimerReturn(std::function<double()> methodUnderTest) {
        double expected = timePointToMilliseconds(steady_clock::now());
        double measured = methodUnderTest();

        requireWithinMargin(expected, measured);
}


TEST_CASE("DefaultTimerSource::startTimer") {
    SECTION("Returns time since epoch in milliseconds") {
        DefaultTimerSource timer = DefaultTimerSource();
        std::function<double(void)> methodUnderTest
            = std::bind(&DefaultTimerSource::startTimer, timer);

        checkTimerReturn(methodUnderTest);
    }
}

TEST_CASE("DefaultTimerSource::readTimer") {
    SECTION("Returns time since epoch in milliseconds") {
        DefaultTimerSource timer = DefaultTimerSource();
        std::function<double(void)> methodUnderTest
            = std::bind(&DefaultTimerSource::readTimer, timer);

        checkTimerReturn(methodUnderTest);
    }
}

TEST_CASE("DefaultTimerSource::stopTimer") {
    SECTION("Returns time since epoch in milliseconds") {
        DefaultTimerSource timer = DefaultTimerSource();
        std::function<double(void)> methodUnderTest
            = std::bind(&DefaultTimerSource::stopTimer, timer);

        checkTimerReturn(methodUnderTest);
    }
}

TEST_CASE("DefaultTimerSource::waitForTime") {
    SECTION("Waits for remaining time") {
        DefaultTimerSource timer = DefaultTimerSource();

        double minWaitTimeInMS = WAIT_TIME_MS;
        double minWaitTimeInSeconds = WAIT_TIME_MS / 1000.;
        timer.setMinimumWaitTime(minWaitTimeInSeconds);

        double startTime = timer.startTimer();

        double actualWaitTimeInSeconds = minWaitTimeInSeconds / 2.;
        double actualWaitTimeInMS = 1000. * actualWaitTimeInSeconds;
        auto sleepTime =
            std::chrono::duration<double, std::ratio<1>>
                (actualWaitTimeInSeconds);
        std::this_thread::sleep_for(sleepTime);
        double sleepTimeMS = durationToMilliseconds(sleepTime);

        double stopTime = timer.stopTimer();
        double waitedTime = timer.waitForTime();
        double currentTime = timePointToMilliseconds(steady_clock::now());

        double expectedTotalTime = waitedTime + (stopTime - startTime);

        requireWithinMargin(expectedTotalTime, minWaitTimeInMS);
        requireWithinMargin(currentTime - minWaitTimeInMS, startTime);
        requireWithinMargin(stopTime - startTime, actualWaitTimeInMS);
        requireWithinMargin(waitedTime, actualWaitTimeInMS);
    }

    SECTION("Does not wait if time has overrun") {
        DefaultTimerSource timer = DefaultTimerSource();

        double minWaitTimeInMS = WAIT_TIME_MS;
        double minWaitTimeInSeconds = WAIT_TIME_MS / 1000.;
        timer.setMinimumWaitTime(minWaitTimeInSeconds);

        double startTime = timer.startTimer();

        double actualWaitTimeInSeconds = 2. * minWaitTimeInSeconds;
        double actualWaitTimeInMS = 1000. * actualWaitTimeInSeconds;
        auto sleepTime =
            std::chrono::duration<double, std::ratio<1>>
                (actualWaitTimeInSeconds);
        std::this_thread::sleep_for(sleepTime);

        double stopTime = timer.stopTimer();
        double waitedTime = timer.waitForTime();
        double currentTime = timePointToMilliseconds(steady_clock::now());

        REQUIRE(waitedTime == 0.);
        requireWithinMargin(currentTime - startTime, actualWaitTimeInMS);
    }

    SECTION("Does not wait if minimum wait time has not been set") {
        DefaultTimerSource timer = DefaultTimerSource();

        double startTime = timer.startTimer();

        double actualWaitTimeInMS = WAIT_TIME_MS;
        double actualWaitTimeInSeconds = WAIT_TIME_MS / 1000.;
        auto sleepTime =
            std::chrono::duration<double, std::ratio<1>>
                (actualWaitTimeInSeconds);
        std::this_thread::sleep_for(sleepTime);

        double stopTime = timer.stopTimer();
        double waitedTime = timer.waitForTime();
        double currentTime = timePointToMilliseconds(steady_clock::now());

        REQUIRE(waitedTime == 0.);
        requireWithinMargin(currentTime - startTime, actualWaitTimeInMS);
    }

    SECTION("Does not wait if timers were not run") {
        DefaultTimerSource timer = DefaultTimerSource();

        double startTime = timePointToMilliseconds(steady_clock::now());
        double waitedTime = timer.waitForTime();
        double currentTime = timePointToMilliseconds(steady_clock::now());

        REQUIRE(waitedTime == 0.);
        requireWithinMargin(currentTime - startTime, 0.);
    }
}
