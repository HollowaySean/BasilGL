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

void testControllerState(
        ProcessController controller,
        std::shared_ptr<ProcessInstance> instance,
        ProcessPrivilege privilege,
        ProcessState state,
        ProcessControllerState expected) {
    instance->privilegeLevel = privilege;
    instance->process->setCurrentState(state);

    controller.interpretProcessState(instance);
    REQUIRE(controller.getCurrentState() == expected);
}

TEST_CASE("Process_ProcessController_interpretProcessState") {
    ProcessController controller = ProcessController();
    std::shared_ptr<IProcess> process = std::make_shared<TestProcess>();
    auto instance = std::make_shared<ProcessInstance>(process);

    SECTION("Kills controller if requested with high permission") {
        testControllerState(controller, instance,
            ProcessPrivilege::HIGH,
            ProcessState::REQUEST_KILL,
            ProcessControllerState::KILLED);
    }

    SECTION("Stops controller if requested kill with low permission") {
        testControllerState(controller, instance,
            ProcessPrivilege::LOW,
            ProcessState::REQUEST_KILL,
            ProcessControllerState::STOPPING);
    }

    SECTION("Does nothing if requested kill with no permission") {
        testControllerState(controller, instance,
            ProcessPrivilege::NONE,
            ProcessState::REQUEST_KILL,
            ProcessControllerState::READY);
    }

    SECTION("Stops if requested stop with high permission") {
        testControllerState(controller, instance,
            ProcessPrivilege::HIGH,
            ProcessState::REQUEST_STOP,
            ProcessControllerState::STOPPING);
    }

    SECTION("Stops if requested stop with low permission") {
        testControllerState(controller, instance,
            ProcessPrivilege::LOW,
            ProcessState::REQUEST_STOP,
            ProcessControllerState::STOPPING);
    }

    SECTION("Does nothing if requested stop with no permission") {
        testControllerState(controller, instance,
            ProcessPrivilege::NONE,
            ProcessState::REQUEST_STOP,
            ProcessControllerState::READY);
    }

    SECTION("Removes process if requested") {
        ProcessSchedule& schedule = controller.schedule;

        schedule.addProcess(instance);
        REQUIRE(schedule.size() == 1);

        instance->process->setCurrentState(ProcessState::REMOVE_PROCESS);
        controller.interpretProcessState(instance);

        REQUIRE(schedule.size() == 0);
    }
}
