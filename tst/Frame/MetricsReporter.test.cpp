#include <catch.hpp>

#include "MetricsReporter.hpp"

TEST_CASE("Frame_MetricsReporter_onLoop") {
    TimerRecord record = TimerRecord();
    FrameMetrics metrics = FrameMetrics();
    std::ostringstream ostream;

    MetricsReporter reporter = MetricsReporter(
        &metrics, 1, Level::INFO, ostream);

    SECTION("Does not log on first frame") {
        metrics.pushTimerRecord(record);
        reporter.onLoop();

        REQUIRE(ostream.str() == "");
    }

    SECTION("Logs at interval set by regularity value") {
        reporter.regularity = 2;

        // Frame 0 (no readout)
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(ostream.str() == "");
        ostream.str("");

        // Frame 1 (no readout)
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(ostream.str() == "");
        ostream.str("");

        // Frame 2 (readout)
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE_FALSE(ostream.str() == "");
        ostream.str("");

        // Frame 3 (no readout)
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(ostream.str() == "");
        ostream.str("");

        // Frame 4 (readout)
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE_FALSE(ostream.str() == "");
    }
}
