#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::MetricsRecord;
using basil::ProcessInstance;

using ms = std::chrono::milliseconds;

TEST_CASE("Process_MetricsRecord_operator") {
    auto process = std::make_shared<TestProcess>();
    auto instance1 = std::make_shared<ProcessInstance>(process);
    auto instance2 = std::make_shared<ProcessInstance>(process);
    auto instance3 = std::make_shared<ProcessInstance>(process);

    MetricsRecord firstRecord = MetricsRecord();
    firstRecord.frameID = 1;
    firstRecord.frameTime = ms(100);
    firstRecord.workTime = ms(50);
    firstRecord.processTimes.emplace(instance1, ms(30));
    firstRecord.processTimes.emplace(instance2, ms(20));

    MetricsRecord secondRecord = MetricsRecord();
    secondRecord.frameID = 2;
    secondRecord.frameTime = ms(80);
    secondRecord.workTime = ms(40);
    secondRecord.processTimes.emplace(instance1, ms(25));
    secondRecord.processTimes.emplace(instance3, ms(15));

    SECTION("operator+ adds subfields") {
        MetricsRecord result = firstRecord + secondRecord;

        CHECK(result.frameID      == 2);
        CHECK(result.frameTime    == ms(180));
        CHECK(result.workTime     == ms(90));

        CHECK(result.processTimes.size()      == 3);
        CHECK(result.processTimes[instance1]  == ms(55));
        CHECK(result.processTimes[instance2]  == ms(20));
        CHECK(result.processTimes[instance3]  == ms(15));
    }

    SECTION("operator- subtracts subfields") {
        MetricsRecord result = firstRecord - secondRecord;

        CHECK(result.frameID      == 2);
        CHECK(result.frameTime    == ms(20));
        CHECK(result.workTime     == ms(10));

        CHECK(result.processTimes.size()      == 2);
        CHECK(result.processTimes[instance1]  == ms(5));
        CHECK(result.processTimes[instance2]  == ms(20));
    }

    SECTION("operator/ divides by integer") {
        MetricsRecord result = firstRecord / 5;

        CHECK(result.frameID      == 1);
        CHECK(result.frameTime    == ms(20));
        CHECK(result.workTime     == ms(10));

        CHECK(result.processTimes.size()      == 2);
        CHECK(result.processTimes[instance1]  == ms(6));
        CHECK(result.processTimes[instance2]  == ms(4));
    }

    SECTION("operator== and operator!=") {
        MetricsRecord equalRecord = firstRecord;

        CHECK(firstRecord.isEqual(equalRecord));
        CHECK(equalRecord.isEqual(firstRecord));

        CHECK_FALSE(firstRecord.isEqual(secondRecord));
        CHECK_FALSE(secondRecord.isEqual(firstRecord));
    }
}

TEST_CASE("Process_MetricsRecord_getFrameRate") {
    MetricsRecord record = MetricsRecord();
    record.frameTime = ms(100);

    SECTION("Calculates from frame time") {
        CHECK(record.getFrameRate() == 10.);
    }
}

TEST_CASE("Process_MetricsRecord_getUncappedFrameRate") {
    MetricsRecord record = MetricsRecord();
    record.workTime = ms(50);

    SECTION("Calculates from work time") {
        CHECK(record.getUncappedFrameRate() == 20.);
    }
}
