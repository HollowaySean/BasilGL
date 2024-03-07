#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::MetricsRecord;
using basil::ProcessInstance;

TestClock::duration getDuration(unsigned int durationInMS) {
    return std::chrono::milliseconds(durationInMS);
}

TEST_CASE("Process_MetricsRecord_operator") {
    auto process = std::make_shared<TestProcess>();
    auto instance1 = std::make_shared<ProcessInstance>(process);
    auto instance2 = std::make_shared<ProcessInstance>(process);
    auto instance3 = std::make_shared<ProcessInstance>(process);

    MetricsRecord firstRecord = MetricsRecord();
    firstRecord.frameID = 1;
    firstRecord.frameTime = getDuration(100);
    firstRecord.workTime = getDuration(50);
    firstRecord.processTimes.emplace(instance1, getDuration(30));
    firstRecord.processTimes.emplace(instance2, getDuration(20));

    MetricsRecord secondRecord = MetricsRecord();
    secondRecord.frameID = 0;
    secondRecord.frameTime = getDuration(80);
    secondRecord.workTime = getDuration(40);
    secondRecord.processTimes.emplace(instance1, getDuration(25));
    secondRecord.processTimes.emplace(instance3, getDuration(15));

    SECTION("operator+ adds subfields") {
        MetricsRecord result = firstRecord + secondRecord;

        REQUIRE(result.frameID      == 1);
        REQUIRE(result.frameTime    == getDuration(180));
        REQUIRE(result.workTime     == getDuration(90));

        REQUIRE(result.processTimes.size()      == 3);
        REQUIRE(result.processTimes[instance1]  == getDuration(55));
        REQUIRE(result.processTimes[instance2]  == getDuration(20));
        REQUIRE(result.processTimes[instance3]  == getDuration(15));
    }

    SECTION("operator- subtracts subfields") {
        MetricsRecord result = firstRecord - secondRecord;

        REQUIRE(result.frameID      == 1);
        REQUIRE(result.frameTime    == getDuration(20));
        REQUIRE(result.workTime     == getDuration(10));

        REQUIRE(result.processTimes.size()      == 2);
        REQUIRE(result.processTimes[instance1]  == getDuration(5));
        REQUIRE(result.processTimes[instance2]  == getDuration(20));
    }
}
