#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::Logger;
using basil::LogLevel;
using basil::MetricsRecord;
using basil::MetricsReporter;
using basil::MetricsObserver;
using basil::ProcessInstance;

TEST_CASE("Process_MetricsReporter_onLoop") {
    auto controller = std::make_shared<ProcessController>();
    MetricsObserver& observer = controller->getMetricsObserver();

    auto process = std::make_shared<TestProcess>();
    auto instance = std::make_shared<ProcessInstance>(process);

    MetricsReporter reporter = MetricsReporter();
    reporter.onRegister(controller.get());
    reporter.regularity = 10;

    MetricsRecord record = MetricsRecord();
    record.frameID = 0;
    record.frameTime = std::chrono::milliseconds(100);
    record.workTime = std::chrono::milliseconds(50);
    record.processTimes.emplace(instance, std::chrono::milliseconds(20));


    Logger& logger = Logger::get();

    SECTION("Does not log on first frame") {
        record.frameID = 0;
        observer.buffer.emplace(record);
        observer.sum = record;

        logger.clearTestInfo();
        reporter.onLoop();
        REQUIRE(logger.getLastOutput() == "");
    }

    SECTION("Does not log on non-multiple frame of regularity") {
        record.frameID = 11;
        observer.buffer.emplace(record);
        observer.sum = record;

        logger.clearTestInfo();
        reporter.onLoop();
        REQUIRE(logger.getLastOutput() == "");
    }

    SECTION("Logs on multiple frame of regularity") {
        record.frameID = 10;
        observer.buffer.emplace(record);
        observer.sum = record;

        logger.clearTestInfo();
        reporter.onLoop();
        REQUIRE_FALSE(logger.getLastOutput() == "");
    }
}

TEST_CASE("Process_MetricsReporter_Builder") {
    SECTION("Builds MetricsReporter object") {
        auto reporter = MetricsReporter::Builder()
            .withLogLevel(LogLevel::DEBUG)
            .withRegularity(25)
            .build();

        REQUIRE(reporter->regularity == 25);
        REQUIRE(reporter->logLevel == LogLevel::DEBUG);
    }
}
