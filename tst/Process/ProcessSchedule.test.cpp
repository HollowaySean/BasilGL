#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::IProcess;
using basil::ProcessInstance;
using basil::ProcessOrdinal;
using basil::ProcessSchedule;

TEST_CASE("Process_ProcessSchedule_addProcess") {
    ProcessSchedule schedule = ProcessSchedule();
    auto process = std::make_shared<TestProcess>();

    auto instance =
        std::make_shared<ProcessInstance>(process);
    auto otherInstance =
        std::make_shared<ProcessInstance>(process);
    schedule.addProcess(otherInstance);

    SECTION("Does nothing if provided null") {
        CHECK(schedule.size() == 1);
        schedule.addProcess(nullptr);
        CHECK(schedule.size() == 1);
    }

    SECTION("Adds early process") {
        instance->ordinal = ProcessOrdinal::EARLY;
        schedule.addProcess(instance);

        CHECK(schedule.early.back() == instance);

        CHECK(schedule.front() == instance);
        CHECK(schedule.back() == otherInstance);
    }

    SECTION("Adds main process") {
        schedule.addProcess(instance);

        CHECK(schedule.main.back() == instance);

        CHECK(schedule.front() == otherInstance);
        CHECK(schedule.back() == instance);
    }

    SECTION("Adds early process") {
        instance->ordinal = ProcessOrdinal::LATE;
        schedule.addProcess(instance);

        CHECK(schedule.late.back() == instance);

        CHECK(schedule.front() == otherInstance);
        CHECK(schedule.back() == instance);
    }
}

TEST_CASE("Process_ProcessSchedule_back") {
    ProcessSchedule schedule = ProcessSchedule();
    auto process = std::make_shared<TestProcess>();

    auto instance =
        std::make_shared<ProcessInstance>(process);
    auto secondInstance =
        std::make_shared<ProcessInstance>(process);

    SECTION("Returns early if no main or late") {
        instance->ordinal = ProcessOrdinal::EARLY;
        schedule.addProcess(instance);

        CHECK(schedule.back() == instance);
    }

    SECTION("Returns main if no late") {
        instance->ordinal = ProcessOrdinal::MAIN;
        schedule.addProcess(instance);
        secondInstance->ordinal = ProcessOrdinal::EARLY;
        schedule.addProcess(secondInstance);

        CHECK(schedule.back() == instance);
    }

    SECTION("Returns late otherwise") {
        instance->ordinal = ProcessOrdinal::LATE;
        schedule.addProcess(instance);
        secondInstance->ordinal = ProcessOrdinal::MAIN;
        schedule.addProcess(secondInstance);

        CHECK(schedule.back() == instance);
    }
}

TEST_CASE("Process_ProcessSchedule_removeProcess") {
    ProcessSchedule schedule = ProcessSchedule();
    auto process = std::make_shared<TestProcess>();
    auto firstInstance =
        std::make_shared<ProcessInstance>(process);
    auto secondInstance =
        std::make_shared<ProcessInstance>(process);

    schedule.addProcess(firstInstance);
    schedule.addProcess(secondInstance);

    SECTION("Does nothing if provided null") {
        CHECK(schedule.size() == 2);

        schedule.removeProcess(nullptr);

        CHECK(schedule.size() == 2);
    }

    SECTION("Removes process from schedule") {
        CHECK(schedule.size() == 2);
        CHECK(schedule.front() == firstInstance);

        schedule.removeProcess(firstInstance);

        CHECK(schedule.size() == 1);
        CHECK(schedule.front() == secondInstance);
    }
}
