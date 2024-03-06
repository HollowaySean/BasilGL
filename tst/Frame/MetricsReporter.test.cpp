#include <sstream>

#include <catch.hpp>

#include <Basil/Frame.hpp>

using basil::Logger;
using basil::TimerRecord;
using basil::MetricsReporter;
using basil::FrameMetrics;
using basil::LogLevel;

TEST_CASE("Frame_MetricsReporter_onLoop") {
    TimerRecord record = TimerRecord();
    record.processStart.emplace(0, 0);
    record.processDone.emplace(0, 1);

    FrameMetrics metrics = FrameMetrics();
    std::map<int, std::string> processNames;
    processNames.emplace(0, "processName");
    metrics.setProcessNames(&processNames);

    MetricsReporter reporter = MetricsReporter(
        &metrics, 1, LogLevel::INFO);

    Logger& logger = Logger::get();

    SECTION("Does not log on first frame") {
        logger.clearTestInfo();

        metrics.pushTimerRecord(record);
        reporter.onLoop();

        REQUIRE(logger.getLastOutput() == "");
    }

    SECTION("Logs at interval set by regularity value") {
        logger.clearTestInfo();
        reporter.regularity = 2;

        // Frame 0 (no readout)
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(logger.getLastOutput() == "");

        // Frame 1 (no readout)
        logger.clearTestInfo();
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(logger.getLastOutput() == "");

        // Frame 2 (readout)
        logger.clearTestInfo();
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE_FALSE(logger.getLastOutput()== "");

        // Frame 3 (no readout)
        logger.clearTestInfo();
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(logger.getLastOutput() == "");

        // Frame 4 (readout)
        logger.clearTestInfo();
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE_FALSE(logger.getLastOutput() == "");
    }
}
