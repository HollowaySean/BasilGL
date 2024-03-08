#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::MetricsObserver;
using basil::MetricsRecord;
using basil::ProcessInstance;

TEST_CASE("Process_MetricsObserver_recordFrameStart") {
    MetricsObserver metrics = MetricsObserver();
    REQUIRE(metrics.currentFrame == 0);

    auto time = FrameClock::now();
    metrics.recordFrameStart(time);

    SECTION("Records frame time") {
        REQUIRE(metrics.frameStartTime == time);
    }

    SECTION("Increments frame counter") {
        REQUIRE(metrics.currentFrame == 1);
    }
}

TEST_CASE("Process_MetricsObserver_recordProcessTime") {
    MetricsObserver metrics = MetricsObserver();
    auto process = std::make_shared<TestProcess>();

    auto instance1 = std::make_shared<ProcessInstance>(process);
    auto instance2 = std::make_shared<ProcessInstance>(process);

    auto time1 = std::chrono::milliseconds(10);
    auto time2 = std::chrono::milliseconds(20);

    SECTION("Records time in process map") {
        MetricsRecord& currentRecord = metrics.current;
        REQUIRE(currentRecord.processTimes.size() == 0);

        metrics.recordProcessTime(instance1, time1);
        metrics.recordProcessTime(instance2, time2);

        REQUIRE(currentRecord.processTimes.size() == 2);
        REQUIRE(currentRecord.processTimes[instance1] == time1);
        REQUIRE(currentRecord.processTimes[instance2] == time2);
    }
}

TEST_CASE("Process_MetricsObserver_recordWorkEnd") {
    MetricsObserver metrics = MetricsObserver();

    auto start = FrameClock::now();
    metrics.recordFrameStart(start);

    auto duration = std::chrono::milliseconds(100);
    auto stop = start + duration;
    metrics.recordWorkEnd(stop);

    SECTION("Records work duration in record") {
        REQUIRE(metrics.current.workTime == duration);
    }
}

TEST_CASE("Process_MetricsObserver_recordFrameEnd") {
    MetricsObserver metrics = MetricsObserver();

    auto start = FrameClock::now();
    metrics.recordFrameStart(start);

    auto duration = std::chrono::milliseconds(100);
    auto stop = start + duration;
    metrics.recordFrameEnd(stop);

    SECTION("Records frame duration in record") {
        REQUIRE(metrics.current.frameTime == duration);
    }
}

TEST_CASE("Process_MetricsObserver_getCurrentMetrics") {
    MetricsObserver metrics = MetricsObserver();

    metrics.sum.frameTime = std::chrono::milliseconds(100);
    metrics.sum.workTime = std::chrono::milliseconds(80);
    metrics.sum.frameID = 10;

    MetricsRecord dummy1 = MetricsRecord();
    MetricsRecord dummy2 = MetricsRecord();

    metrics.buffer.emplace(dummy1);
    metrics.buffer.emplace(dummy2);

    SECTION("Averages over buffer") {
        MetricsRecord output = metrics.getCurrentMetrics();

        REQUIRE(output.frameID == 10);
        REQUIRE(output.frameTime == std::chrono::milliseconds(50));
        REQUIRE(output.workTime == std::chrono::milliseconds(40));
    }
}

TEST_CASE("Process_MetricsObserver_getLatestMetrics") {
    MetricsObserver metrics = MetricsObserver();

    MetricsRecord record1 = MetricsRecord();
    record1.frameTime = std::chrono::milliseconds(100);
    record1.workTime = std::chrono::milliseconds(80);
    record1.frameID = 10;

    MetricsRecord record2 = MetricsRecord();
    record2.frameTime = std::chrono::milliseconds(40);
    record2.workTime = std::chrono::milliseconds(30);
    record2.frameID = 15;

    metrics.buffer.emplace(record1);
    metrics.buffer.emplace(record2);

    SECTION("Returns most recent metrics") {
        auto result = metrics.getLatestMetrics();

        REQUIRE(result.isEqual(record2));
        REQUIRE_FALSE(result.isEqual(record1));

        metrics.buffer.emplace(record1);

        result = metrics.getLatestMetrics();

        REQUIRE(result.isEqual(record1));
    }
}

TEST_CASE("Process_MetricsObserver_setBufferSize") {
    MetricsObserver metrics = MetricsObserver();
    metrics.bufferSize = 2;

    MetricsRecord record1 = MetricsRecord();
    record1.frameTime = std::chrono::milliseconds(100);
    record1.workTime = std::chrono::milliseconds(80);
    record1.frameID = 10;

    MetricsRecord record2 = MetricsRecord();
    record2.frameTime = std::chrono::milliseconds(40);
    record2.workTime = std::chrono::milliseconds(30);
    record2.frameID = 15;

    metrics.buffer.emplace(record1);
    metrics.buffer.emplace(record2);

    SECTION("No op if provided zero") {
        metrics.setBufferSize(0);

        REQUIRE(metrics.getBufferSize() == 2);
        REQUIRE(metrics.getBufferCount() == 2);
    }

    SECTION("Increases non-destructively") {
        metrics.setBufferSize(5);

        REQUIRE(metrics.getBufferSize() == 5);
        REQUIRE(metrics.getBufferCount() == 2);
    }

    SECTION("Decreases destructively") {
        metrics.setBufferSize(1);

        REQUIRE(metrics.getBufferSize() == 1);
        REQUIRE(metrics.getBufferCount() == 1);
    }
}
