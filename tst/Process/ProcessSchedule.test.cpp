#include <catch.hpp>

#include <Basil/Frame.hpp>
#include <Basil/Process.hpp>

using basil::IFrameProcess;
using basil::ProcessInstance;
using basil::ProcessOrdinal;
using basil::ProcessSchedule;

class TestProcess : public IFrameProcess {
    void onLoop() override {}
};

TEST_CASE("Process_ProcessSchedule_addProcess") {
    ProcessSchedule schedule = ProcessSchedule();
    TestProcess process = TestProcess();

    auto instance =
        std::make_shared<ProcessInstance>(process);
    auto otherInstance =
        std::make_shared<ProcessInstance>(process);
    schedule.addProcess(otherInstance);

    SECTION("Does nothing if provided null") {
        REQUIRE(schedule.size() == 1);
        schedule.addProcess(nullptr);
        REQUIRE(schedule.size() == 1);
    }

    SECTION("Adds early process") {
        instance->ordinal = ProcessOrdinal::EARLY;
        schedule.addProcess(instance);

        REQUIRE(schedule.early.back() == instance);

        REQUIRE(schedule.front() == instance);
        REQUIRE(schedule.back() == otherInstance);
    }

    SECTION("Adds main process") {
        schedule.addProcess(instance);

        REQUIRE(schedule.main.back() == instance);

        REQUIRE(schedule.front() == otherInstance);
        REQUIRE(schedule.back() == instance);
    }

    SECTION("Adds early process") {
        instance->ordinal = ProcessOrdinal::LATE;
        schedule.addProcess(instance);

        REQUIRE(schedule.late.back() == instance);

        REQUIRE(schedule.front() == otherInstance);
        REQUIRE(schedule.back() == instance);
    }
}

TEST_CASE("Process_ProcessSchedule_back") {
    ProcessSchedule schedule = ProcessSchedule();
    TestProcess process = TestProcess();

    auto instance =
        std::make_shared<ProcessInstance>(process);
    auto secondInstance =
        std::make_shared<ProcessInstance>(process);

    SECTION("Returns early if no main or late") {
        instance->ordinal = ProcessOrdinal::EARLY;
        schedule.addProcess(instance);

        REQUIRE(schedule.back() == instance);
    }

    SECTION("Returns main if no late") {
        instance->ordinal = ProcessOrdinal::MAIN;
        schedule.addProcess(instance);
        secondInstance->ordinal = ProcessOrdinal::EARLY;
        schedule.addProcess(secondInstance);

        REQUIRE(schedule.back() == instance);
    }

    SECTION("Returns late otherwise") {
        instance->ordinal = ProcessOrdinal::LATE;
        schedule.addProcess(instance);
        secondInstance->ordinal = ProcessOrdinal::MAIN;
        schedule.addProcess(secondInstance);

        REQUIRE(schedule.back() == instance);
    }
}

TEST_CASE("Process_ProcessSchedule_removeProcess") {
    ProcessSchedule schedule = ProcessSchedule();
    TestProcess process = TestProcess();
    auto firstInstance =
        std::make_shared<ProcessInstance>(process);
    auto secondInstance =
        std::make_shared<ProcessInstance>(process);

    schedule.addProcess(firstInstance);
    schedule.addProcess(secondInstance);

    SECTION("Does nothing if provided null") {
        REQUIRE(schedule.size() == 2);

        schedule.removeProcess(nullptr);

        REQUIRE(schedule.size() == 2);
    }

    SECTION("Removes process from schedule") {
        REQUIRE(schedule.size() == 2);
        REQUIRE(schedule.front() == firstInstance);

        schedule.removeProcess(firstInstance);

        REQUIRE(schedule.size() == 1);
        REQUIRE(schedule.front() == secondInstance);
    }
}
