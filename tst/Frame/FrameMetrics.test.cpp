#include <catch.hpp>

#include <Basil/Frame.hpp>

class TestTimer : public ITimerSource {
 public:
    TimerRecord testRecord;
    TimerRecord getRecord() const override {
        return testRecord;
    }
    std::unique_ptr<ITimerSource> clone() const override {
        return std::make_unique<TestTimer>(*this);
    }
    void frameStart() override {}
    void frameDone() override {}
    void waitForFrameTime() override {}
    void processStart(int pid) override {}
    void processDone(int pid) override {}
    double readTimer() const override { return 0.; }
};

bool floatCompare(double expected, double actual) {
    constexpr double EPSILON = 1e-9;

    return (expected - actual <= EPSILON)
        && (actual - expected <= EPSILON);
}

TEST_CASE("Frame_FrameMetricsRecord_getFrameRate") {
    SECTION("Calculates frame rate from frame time") {
        FrameMetricsRecord record = FrameMetricsRecord();
        record.frameTime = 0.100;
        record.workTime = 0.050;

        REQUIRE(record.getFrameRate() == 10.);
    }
}

TEST_CASE("Frame_FrameMetricsRecord_getUncappedFrameRate") {
    SECTION("Calculates frame rate from work time") {
        FrameMetricsRecord record = FrameMetricsRecord();
        record.frameTime = 0.100;
        record.workTime = 0.050;

        REQUIRE(record.getUncappedFrameRate() == 20.);
    }
}

TEST_CASE("Frame_FrameMetricsRecord_fromTimestamp") {
    TimerRecord record = TimerRecord();
    record.frameStart = 0.031;
    record.frameDone = 0.081;
    record.frameEnd = 0.131;
    record.processStart.emplace(0, 0.035);
    record.processDone.emplace(0, 0.055);
    record.processStart.emplace(1, 0.040);
    record.processDone.emplace(2, 0.055);

    FrameMetricsRecord metricsRecord =
        FrameMetricsRecord::fromTimestamp(record);

    SECTION("Calculates frame time") {
        REQUIRE(floatCompare(metricsRecord.frameTime, 0.100));
    }

    SECTION("Calculates work time") {
        REQUIRE(floatCompare(metricsRecord.workTime, 0.050));
    }

    SECTION("Calculates process time") {
        REQUIRE(floatCompare(metricsRecord.processTime[0], 0.020));
    }

    SECTION("Does not calculate incomplete process time") {
        REQUIRE(metricsRecord.processTime.count(1) == 0);
        REQUIRE(metricsRecord.processTime.count(2) == 0);
    }
}

TEST_CASE("Frame_FrameMetricsRecord_getExcludingCurrent") {
    SECTION("Returns default if buffer is empty") {
        FrameMetrics metrics = FrameMetrics();
        FrameMetricsRecord record = metrics.getExcludingCurrent();

        REQUIRE(record.frameID == 0);
        REQUIRE(record.frameTime == 0.);
        REQUIRE(record.workTime == 0.);
        REQUIRE(record.processTime.size() == 0);
    }

    SECTION("Returns all but latest in buffer") {
        FrameMetrics metrics = FrameMetrics(2);

        // First update, which should be returned
        TimerRecord record = TimerRecord(0);
        record.frameID = 0;
        record.frameStart = 0.000;
        record.frameDone = 0.020;
        record.frameEnd = 0.050;
        record.processStart.emplace(1, 0.000);
        record.processDone.emplace(1, 0.004);
        record.processStart.emplace(2, 0.010);
        record.processDone.emplace(2, 0.020);
        metrics.pushTimerRecord(record);

        // Second update, which should be discarded
        record = TimerRecord(0);
        record.frameID = 0;
        record.frameStart = 0.00;
        record.frameDone = 0.010;
        record.frameEnd = 0.040;
        record.processStart.emplace(1, 0.000);
        record.processDone.emplace(1, 0.002);
        record.processStart.emplace(2, 0.005);
        record.processDone.emplace(2, 0.009);
        metrics.pushTimerRecord(record);


        FrameMetricsRecord result = metrics.getExcludingCurrent();

        REQUIRE(floatCompare(result.frameTime, 0.050));
        REQUIRE(floatCompare(result.workTime, 0.020));
        REQUIRE(floatCompare(result.processTime[1], 0.004));
        REQUIRE(floatCompare(result.processTime[2], 0.010));
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
        record.frameStart = 0.000;
        record.frameDone = 0.020;
        record.frameEnd = 0.050;
        record.processStart.emplace(1, 0.000);
        record.processDone.emplace(1, 0.004);
        record.processStart.emplace(2, 0.010);
        record.processDone.emplace(2, 0.020);
        metrics.pushTimerRecord(record);

        FrameMetricsRecord result = metrics.getCurrentMetrics();

        REQUIRE(floatCompare(result.frameTime, 0.050));
        REQUIRE(floatCompare(result.workTime, 0.020));
        REQUIRE(floatCompare(result.processTime[1], 0.004));
        REQUIRE(floatCompare(result.processTime[2], 0.010));

        // Second update, should return averaged values
        record = TimerRecord(0);
        record.frameID = 0;
        record.frameStart = 0.00;
        record.frameDone = 0.010;
        record.frameEnd = 0.040;
        record.processStart.emplace(1, 0.000);
        record.processDone.emplace(1, 0.002);
        record.processStart.emplace(2, 0.005);
        record.processDone.emplace(2, 0.009);
        metrics.pushTimerRecord(record);

        result = metrics.getCurrentMetrics();

        REQUIRE(floatCompare(result.frameTime, 0.045));
        REQUIRE(floatCompare(result.workTime, 0.015));
        REQUIRE(floatCompare(result.processTime[1], 0.003));
        REQUIRE(floatCompare(result.processTime[2], 0.007));

        // Third update, should return averaged values without first update
        record = TimerRecord(0);
        record.frameID = 0;
        record.frameStart = 0.00;
        record.frameDone = 0.010;
        record.frameEnd = 0.040;
        record.processStart.emplace(1, 0.000);
        record.processDone.emplace(1, 0.002);
        record.processStart.emplace(2, 0.005);
        record.processDone.emplace(2, 0.009);
        metrics.pushTimerRecord(record);

        result = metrics.getCurrentMetrics();

        REQUIRE(floatCompare(result.frameTime, 0.040));
        REQUIRE(floatCompare(result.workTime, 0.010));
        REQUIRE(floatCompare(result.processTime[1], 0.002));
        REQUIRE(floatCompare(result.processTime[2], 0.004));
    }
}

TEST_CASE("Frame_FrameMetrics_pushRecordFromTimer") {
    SECTION("Pushes record from timer") {
        FrameMetrics metrics = FrameMetrics(2);

        // First update, should return identical values
        TimerRecord record = TimerRecord(0);
        record.frameID = 0;
        record.frameStart = 0.000;
        record.frameDone = 0.020;
        record.frameEnd = 0.050;
        record.processStart.emplace(1, 0.000);
        record.processDone.emplace(1, 0.004);
        record.processStart.emplace(2, 0.010);
        record.processDone.emplace(2, 0.020);

        TestTimer timer = TestTimer();
        timer.currentRecord = record;

        metrics.pushRecordFromTimer(timer.clone());

        FrameMetricsRecord result = metrics.getCurrentMetrics();

        REQUIRE(floatCompare(result.frameTime, 0.050));
        REQUIRE(floatCompare(result.workTime, 0.020));
        REQUIRE(floatCompare(result.processTime[1], 0.004));
        REQUIRE(floatCompare(result.processTime[2], 0.010));
    }
}

TEST_CASE("Frame_FrameMetrics_getProcessNames") {
    SECTION("Returns map of process names") {
        FrameMetrics frameMetrics = FrameMetrics();

        std::map<int, std::string> names = std::map<int, std::string>();
        names.emplace(0, "process_0");
        names.emplace(1, "process_1");

        frameMetrics.setProcessNames(&names);

        REQUIRE(frameMetrics.getProcessNames() == &names);
    }
}
