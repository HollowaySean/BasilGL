#include <catch.hpp>

#include "Widget/StopAfterTime.hpp"

using basil::FrameClock;
using basil::ProcessState;
using basil::StopAfterTime;

TEST_CASE("Widget_StopAfterTime_onStart") {
    auto widget = StopAfterTime();
    widget.setFramesUntilStop(100);
    widget.setTimeUntilStop(5);

    SECTION("Initializes time/frame limits") {
        widget.onStart();

        CHECK(widget.stopTime < FrameClock::time_point::max());
        CHECK(widget.stopFrame < (unsigned int)-1);
    }
}

TEST_CASE("Widget_StopAfterTime_onLoop") {
    auto widget = StopAfterTime();
    widget.setFramesUntilStop(100);
    widget.onStart();

    SECTION("Does not request stop until limit is passed") {
        CHECK(widget.getCurrentState() == ProcessState::READY);
        widget.onLoop();
        CHECK(widget.getCurrentState() == ProcessState::READY);
        widget.currentFrame = 101;
        widget.onLoop();
        CHECK(widget.getCurrentState() == ProcessState::REQUEST_STOP);
    }
}

TEST_CASE("Widget_StopAfterTime_Builder") {
    auto widget = StopAfterTime::Builder()
        .setFramesUntilStop(100)
        .setTimeUntilStop(5)
        .build();

    SECTION("Builds correctly") {
        CHECK(widget->timeToRun == std::chrono::seconds(5));
        CHECK(widget->framesToRun == 100);
    }
}
