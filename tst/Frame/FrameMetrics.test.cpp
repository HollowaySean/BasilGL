#include <catch.hpp>

#include "FrameMetrics.hpp"

TEST_CASE("Frame_FrameMetricsRecord_getFrameRate") {
    SECTION("Calculates frame rate from frame time") {
        FrameMetricsRecord record = FrameMetricsRecord();
        record.frameTime = 100.;
        record.workTime = 50.;

        REQUIRE(record.getFrameRate() == 10.);
    }
}

TEST_CASE("Frame_FrameMetricsRecord_getUncappedFrameRate") {
    SECTION("Calculates frame rate from work time") {
        FrameMetricsRecord record = FrameMetricsRecord();
        record.frameTime = 100.;
        record.workTime = 50.;

        REQUIRE(record.getUncappedFrameRate() == 20.);
    }
}

TEST_CASE("Frame_FrameMetricsRecord_fromTimestamp") {
    TimerRecord record = TimerRecord();
    record.frameStart = 31.;
    record.frameDone = 81.;
    record.frameEnd = 131.;
    record.processStart.emplace(0, 35.);
    record.processDone.emplace(0, 55.);
    record.processStart.emplace(1, 40.);
    record.processDone.emplace(2, 55.);

    FrameMetricsRecord metricsRecord =
        FrameMetricsRecord::fromTimestamp(record);

    SECTION("Calculates frame time") {
        REQUIRE(metricsRecord.frameTime == 100.);
    }

    SECTION("Calculates work time") {
        REQUIRE(metricsRecord.workTime == 50.);
    }

    SECTION("Calculates process time") {
        REQUIRE(metricsRecord.processTime[0] == 20.);
    }

    SECTION("Does not calculate incomplete process time") {
        REQUIRE(metricsRecord.processTime.count(1) == 0);
        REQUIRE(metricsRecord.processTime.count(2) == 0);
    }
}

TEST_CASE("Frame_FrameMetrics_pushTimerRecord") {
    SECTION("Increases buffer count, up to maximum") {
        FrameMetrics metrics = FrameMetrics(2);

        REQUIRE(metrics.getBufferSize() == 2);
        REQUIRE(metrics.getBufferCount() == 0);

        metrics.pushTimerRecord(TimerRecord());
        REQUIRE(metrics.getBufferCount() == 1);

        metrics.pushTimerRecord(TimerRecord());
        REQUIRE(metrics.getBufferCount() == 2);

        metrics.pushTimerRecord(TimerRecord());
        REQUIRE(metrics.getBufferSize() == 2);
        REQUIRE(metrics.getBufferCount() == 2);
    }

    SECTION("Updates metrics") {
        FrameMetrics metrics = FrameMetrics(2);

        // First update, should return identical values
        TimerRecord record = TimerRecord(0);
        record.frameID = 0;
        record.frameStart = 0.;
        record.frameDone = 20.;
        record.frameEnd = 50.;
        record.processStart.emplace(1, 0.);
        record.processDone.emplace(1, 4.);
        record.processStart.emplace(2, 10.);
        record.processDone.emplace(2, 20.);
        metrics.pushTimerRecord(record);

        FrameMetricsRecord result = metrics.getCurrentMetrics();

        REQUIRE(result.frameTime == 50.);
        REQUIRE(result.workTime == 20.);
        REQUIRE(result.processTime[1] == 4.);
        REQUIRE(result.processTime[2] == 10.);

        // Second update, should return averaged values
        record = TimerRecord(0);
        record.frameID = 0;
        record.frameStart = 0.;
        record.frameDone = 10.;
        record.frameEnd = 40.;
        record.processStart.emplace(1, 0.);
        record.processDone.emplace(1, 2.);
        record.processStart.emplace(2, 5.);
        record.processDone.emplace(2, 9.);
        metrics.pushTimerRecord(record);

        result = metrics.getCurrentMetrics();

        REQUIRE(result.frameTime == 45.);
        REQUIRE(result.workTime == 15.);
        REQUIRE(result.processTime[1] == 3.);
        REQUIRE(result.processTime[2] == 7.);

        // Third update, should return averaged values without first update
        record = TimerRecord(0);
        record.frameID = 0;
        record.frameStart = 0.;
        record.frameDone = 10.;
        record.frameEnd = 40.;
        record.processStart.emplace(1, 0.);
        record.processDone.emplace(1, 2.);
        record.processStart.emplace(2, 5.);
        record.processDone.emplace(2, 9.);
        metrics.pushTimerRecord(record);

        result = metrics.getCurrentMetrics();

        REQUIRE(result.frameTime == 40.);
        REQUIRE(result.workTime == 10.);
        REQUIRE(result.processTime[1] == 2.);
        REQUIRE(result.processTime[2] == 4.);
    }
}
