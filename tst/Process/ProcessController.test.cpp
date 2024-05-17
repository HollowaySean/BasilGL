#include <catch.hpp>

#include "Process/ProcessController.hpp"

#include "Process/ProcessTestUtils.hpp"

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
        CHECK(schedule.main.front()->process == process);
        CHECK(schedule.main.front()->privilegeLevel
            == ProcessPrivilege::HIGH);
    }

    SECTION("Adds process to early") {
        controller.addEarlyProcess(process, ProcessPrivilege::LOW);

        REQUIRE(schedule.size() == 1);
        CHECK(schedule.early.front()->process == process);
        CHECK(schedule.early.front()->privilegeLevel
            == ProcessPrivilege::LOW);
    }

    SECTION("Adds process to late") {
        controller.addLateProcess(process);

        REQUIRE(schedule.size() == 1);
        CHECK(schedule.late.front()->process == process);
        CHECK(schedule.late.front()->privilegeLevel
            == ProcessPrivilege::NONE);
    }
}

TEST_CASE("Process_ProcessController_hasProcess") {
    ProcessController controller = ProcessController();
    std::shared_ptr<IProcess> processOne = std::make_shared<TestProcess>();
    processOne->setProcessName("processOne");
    std::shared_ptr<IProcess> processTwo = std::make_shared<TestProcess>();
    processTwo->setProcessName("processTwo");

    auto instance = controller.addProcess(processOne);

    SECTION("Returns true for process in schedule") {
        CHECK(controller.hasProcess(processOne));
        CHECK(controller.hasProcess(instance->processName));
        CHECK(controller.hasProcess(instance->getID()));
    }

    SECTION("Returns false for process not in schedule") {
        CHECK_FALSE(controller.hasProcess(processTwo));
        CHECK_FALSE(controller.hasProcess("processTwo"));
        CHECK_FALSE(controller.hasProcess(instance->getID() + 1));
    }
}

TEST_CASE("Process_ProcessController_getProcess") {
    ProcessController controller = ProcessController();
    std::shared_ptr<IProcess> processOne = std::make_shared<TestProcess>();
    processOne->setProcessName("processOne");
    std::shared_ptr<IProcess> processTwo = std::make_shared<TestProcess>();
    processTwo->setProcessName("processTwo");

    auto instanceOne = controller.addProcess(processOne);
    auto instanceTwo = controller.addProcess(processTwo);

    SECTION("Returns process instance with name") {
        auto result = controller.getProcess("processTwo");
        CHECK(result.value() == instanceTwo);
    }

    SECTION("Returns empty optional for missing process") {
        auto result = controller.getProcess("processThree");
        CHECK_FALSE(result.has_value());
    }

    SECTION("Returns process instance with ID") {
        auto result = controller.getProcess(instanceTwo->getID());
        CHECK(result.value() == instanceTwo);
    }

    SECTION("Returns empty optional for missing ID") {
        auto result = controller.getProcess(instanceOne->getID() + 1000);
        CHECK_FALSE(result.has_value());
    }
}

TEST_CASE("Process_ProcessController_run") {
    ProcessController controller = ProcessController();
    auto process = std::make_shared<TestProcess>();

    SECTION("Falls through if no processes") {
        CHECK(controller.getCurrentState()
            == ProcessControllerState::READY);

        controller.run();

        CHECK(controller.getCurrentState()
            == ProcessControllerState::STOPPED);
    }

    SECTION("Maintains state throughout run") {
        CHECK(controller.getCurrentState()
            == ProcessControllerState::READY);

        controller.addProcess(process, ProcessPrivilege::HIGH);
        controller.run();

        CHECK(process->didStart);
        CHECK(process->didLoop);
        CHECK(process->didStop);

        CHECK(process->startPCState
            == ProcessControllerState::STARTING);
        CHECK(process->loopPCState
            == ProcessControllerState::RUNNING);
        CHECK(process->stopPCState
            == ProcessControllerState::STOPPING);
        CHECK(controller.getCurrentState()
            == ProcessControllerState::STOPPED);
    }

    SECTION("Does not continue if killed") {
        CHECK(controller.getCurrentState()
            == ProcessControllerState::READY);

        process->stateAfterStart = ProcessState::REQUEST_KILL;

        controller.addProcess(process, ProcessPrivilege::HIGH);
        controller.run();

        CHECK(process->startPCState
            == ProcessControllerState::STARTING);
        CHECK(controller.getCurrentState()
            == ProcessControllerState::KILLED);

        CHECK(process->didStart);
        CHECK_FALSE(process->didLoop);
        CHECK_FALSE(process->didStop);
    }
}

TEST_CASE("Process_ProcessController_stop") {
    SECTION("Sets state to STOPPING") {
        ProcessController controller = ProcessController();

        CHECK(controller.getCurrentState()
            == ProcessControllerState::READY);
        controller.stop();
        CHECK(controller.getCurrentState()
            == ProcessControllerState::STOPPING);
    }
}

TEST_CASE("Process_ProcessController_kill") {
    SECTION("Sets state to KILLED") {
        ProcessController controller = ProcessController();

        CHECK(controller.getCurrentState() == ProcessControllerState::READY);
        controller.kill();
        CHECK(controller.getCurrentState() == ProcessControllerState::KILLED);
    }
}

TEST_CASE("Process_ProcessController_setFrameCap") {
    SECTION("Correctly sets frame time") {
        ProcessController controller = ProcessController();
        controller.setFrameCap(50);

        CHECK(controller.frameCap == 50);
        TestClock::duration durNS = std::chrono::milliseconds(20);
        CHECK(controller.frameTime == durNS);
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
    CHECK(controller.getCurrentState() == expected);
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
        CHECK(schedule.size() == 1);

        instance->process->setCurrentState(ProcessState::REMOVE_PROCESS);
        controller.interpretProcessState(instance);

        CHECK(schedule.size() == 0);
    }
}

TEST_CASE("Process_ProcessController_sleepForRestOfFrame") {
    SECTION("Sleeps until wake time") {
        ProcessController controller = ProcessController();

        TestClock::setNextTimeStampToNow();
        TestClock::time_point frameStartTime = TestClock::now();
        unsigned int frameStartTimeCount =
            frameStartTime.time_since_epoch().count();

        controller.frameTime = std::chrono::milliseconds(100);
        unsigned int frameTimeCount = controller.frameTime.count();

        controller.sleepForRestOfFrame(frameStartTime);

        TestClock::setNextTimeStampToNow();
        TestClock::time_point frameEndTime = TestClock::now();
        unsigned int frameEndTimeCount =
            frameEndTime.time_since_epoch().count();

        CHECK(frameEndTimeCount - frameStartTimeCount >= frameTimeCount);
    }
}

TEST_CASE("Process_ProcessController_shouldRunProcess") {
    ProcessController controller = ProcessController();
    std::shared_ptr<IProcess> process = std::make_shared<TestProcess>();
    auto instance = std::make_shared<ProcessInstance>(process);

    SECTION("Returns true if process and controller are low status") {
        controller.currentState = ProcessControllerState::READY;
        process->currentState = ProcessState::READY;

        CHECK(controller.shouldRunProcess(instance));
    }

    SECTION("Returns false if process is high status") {
        controller.currentState = ProcessControllerState::READY;
        process->currentState = ProcessState::REQUEST_KILL;

        CHECK_FALSE(controller.shouldRunProcess(instance));
    }

    SECTION("Returns false if controller is high status") {
        controller.currentState = ProcessControllerState::KILLED;
        process->currentState = ProcessState::READY;

        CHECK_FALSE(controller.shouldRunProcess(instance));
    }
}

TEST_CASE("Process_ProcessController_shouldContinueLoop") {
    ProcessController controller = ProcessController();
    std::shared_ptr<IProcess> process = std::make_shared<TestProcess>();

    SECTION("Returns true if processes and not stopped") {
        controller.currentState = ProcessControllerState::RUNNING;
        controller.addProcess(process);

        CHECK(controller.shouldContinueLoop());
    }

    SECTION("Returns false if controller is stopped") {
        controller.currentState = ProcessControllerState::STOPPING;
        controller.addProcess(process);

        CHECK_FALSE(controller.shouldContinueLoop());
    }

    SECTION("Returns false if no processes exist") {
        controller.currentState = ProcessControllerState::RUNNING;

        CHECK_FALSE(controller.shouldContinueLoop());
    }
}

TEST_CASE("Process_ProcessController_Builder") {
    SECTION("Builds ProcessController object") {
        auto process1 = std::make_shared<TestProcess>();
        auto process2 = std::make_shared<TestProcess>();
        auto process3 = std::make_shared<TestProcess>();

        auto controller = ProcessController::Builder()
            .withFrameCap(25)
            .withEarlyProcess(process1)
            .withProcess(process2, ProcessPrivilege::LOW)
            .withLateProcess(process3, ProcessPrivilege::HIGH)
            .build();

        CHECK(controller->getFrameCap() == 25);

        CHECK(controller->schedule.early.back()->process == process1);
        CHECK(controller->schedule.main.back()->process == process2);
        CHECK(controller->schedule.late.back()->process == process3);

        CHECK(controller->schedule.early.back()->privilegeLevel
            == ProcessPrivilege::NONE);
        CHECK(controller->schedule.main.back()->privilegeLevel
            == ProcessPrivilege::LOW);
        CHECK(controller->schedule.late.back()->privilegeLevel
            == ProcessPrivilege::HIGH);
    }
}
