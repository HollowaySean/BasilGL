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
    logger.clearTestInfo();

    SECTION("Does not log on first frame") {
        metrics.pushTimerRecord(record);
        reporter.onLoop();

        REQUIRE(logger.getLastOutput() == "");
    }

    SECTION("Logs at interval set by regularity value") {
        reporter.regularity = 2;

        // Frame 0 (no readout)
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(logger.getLastOutput() == "");
        logger.clearTestInfo();

        // Frame 1 (no readout)
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(logger.getLastOutput() == "");
        logger.clearTestInfo();

        // Frame 2 (readout)
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE_FALSE(logger.getLastOutput()== "");
        logger.clearTestInfo();

        // Frame 3 (no readout)
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE(logger.getLastOutput() == "");
        logger.clearTestInfo();

        // Frame 4 (readout)
        record.frameID++;
        metrics.pushTimerRecord(record);
        reporter.onLoop();
        REQUIRE_FALSE(logger.getLastOutput() == "");
    }
}
