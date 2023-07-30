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

void checkTimerReturn(std::function<double()> methodUnderTest) {
        double expected =
            DefaultTimerSource::timePointToMilliseconds(steady_clock::now());
        double measured = methodUnderTest();

        requireWithinMargin(expected, measured);
}


TEST_CASE("Frame_DefaultTimerSource_readTimer") {
    SECTION("Returns current time") {
        DefaultTimerSource timer = DefaultTimerSource();

        std::function<double(void)> methodUnderTest
            = [&]() -> double {
                return timer.readTimer();
            };

        checkTimerReturn(methodUnderTest);
    }
}

TEST_CASE("Frame_DefaultTimerSource_frameStart") {
    SECTION("Sets frame start time in record") {
        DefaultTimerSource timer = DefaultTimerSource();
        std::function<double(void)> methodUnderTest
            = [&]() -> double {
                timer.frameStart();
                return timer.getRecord().frameStart;
            };

        checkTimerReturn(methodUnderTest);
    }

    SECTION("Updates the frame ID") {
        DefaultTimerSource timer = DefaultTimerSource();
        REQUIRE(timer.getRecord().frameID == 0);

        timer.frameStart();
        REQUIRE(timer.getRecord().frameID == 1);

        timer.frameStart();
        REQUIRE(timer.getRecord().frameID == 2);
    }
}

TEST_CASE("Frame_DefaultTimerSource_frameDone") {
    SECTION("Sets frame done time in record") {
        DefaultTimerSource timer = DefaultTimerSource();

        std::function<double(void)> methodUnderTest
            = [&]() -> double {
                timer.frameDone();
                return timer.getRecord().frameDone;
            };

        checkTimerReturn(methodUnderTest);
    }
}

TEST_CASE("Frame_DefaultTimerSource_processStart") {
    SECTION("Sets process start time in record") {
        DefaultTimerSource timer = DefaultTimerSource();

        std::function<double(void)> methodUnderTest
            = [&]() -> double {
                timer.processStart(1);
                return timer.getRecord().processStart[1];
            };

        checkTimerReturn(methodUnderTest);
    }
}

TEST_CASE("Frame_DefaultTimerSource_processDone") {
    SECTION("Sets process done time in record") {
        DefaultTimerSource timer = DefaultTimerSource();

        std::function<double(void)> methodUnderTest
            = [&]() -> double {
                timer.processDone(1);
                return timer.getRecord().processDone[1];
            };

        checkTimerReturn(methodUnderTest);
    }
}

void checkWaitTime(
        DefaultTimerSource timer,
        double elapsedTimeInMS,
        double expectedWaitTimeInMS) {
    timer.frameStart();

    double elapsedTimeInSeconds = elapsedTimeInMS / 1000.;
    auto sleepTime =
        std::chrono::duration<double, std::ratio<1>>
            (elapsedTimeInSeconds);
    std::this_thread::sleep_for(sleepTime);

    timer.frameDone();
    timer.waitForFrameTime();

    double actualWaitTimeInMS =
        timer.getRecord().frameEnd - timer.getRecord().frameDone;

    requireWithinMargin(expectedWaitTimeInMS, actualWaitTimeInMS);
}

TEST_CASE("Frame_DefaultTimerSource_waitForFrameTime") {
    SECTION("Waits for remaining time") {
        DefaultTimerSource timer = DefaultTimerSource();
        timer.setMinimumFrameTime(WAIT_TIME_MS / 1000.);

        checkWaitTime(
            timer,
            WAIT_TIME_MS / 2.,
            WAIT_TIME_MS / 2.);
    }

    SECTION("Does not wait if time has overrun") {
        DefaultTimerSource timer = DefaultTimerSource();
        timer.setMinimumFrameTime(WAIT_TIME_MS / 1000.);

        checkWaitTime(
            timer,
            WAIT_TIME_MS * 2.,
            0.);
    }


    SECTION("Does not wait if minimum wait time has not been set") {
        DefaultTimerSource timer = DefaultTimerSource();

        checkWaitTime(
            timer,
            WAIT_TIME_MS / 2.,
            0.);
    }
}
