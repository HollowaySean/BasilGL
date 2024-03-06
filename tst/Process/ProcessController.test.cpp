#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::ProcessController;
using basil::ProcessControllerState;
using basil::ProcessInstance;
using basil::ProcessPrivilege;
using basil::ProcessSchedule;
using basil::ProcessState;

TEST_CASE("Process_ProcessController_addProcess") {
    ProcessController controller = ProcessController();
    std::shared_ptr<IProcess> process = std::make_shared<TestProcess>();

    ProcessSchedule& schedule = controller.schedule;

    SECTION("Adds process to main") {
        controller.addProcess(process, ProcessPrivilege::HIGH);

        REQUIRE(schedule.size() == 1);
        REQUIRE(schedule.main.front()->process == process);
        REQUIRE(schedule.main.front()->privilegeLevel
            == ProcessPrivilege::HIGH);
    }

    SECTION("Adds process to early") {
        controller.addEarlyProcess(process, ProcessPrivilege::LOW);

        REQUIRE(schedule.size() == 1);
        REQUIRE(schedule.early.front()->process == process);
        REQUIRE(schedule.early.front()->privilegeLevel
            == ProcessPrivilege::LOW);
    }

    SECTION("Adds process to late") {
        controller.addLateProcess(process);

        REQUIRE(schedule.size() == 1);
        REQUIRE(schedule.late.front()->process == process);
        REQUIRE(schedule.late.front()->privilegeLevel
            == ProcessPrivilege::NONE);
    }
}
