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

TEST_CASE("Process_ProcessController_run") {
    ProcessController controller = ProcessController();
    std::shared_ptr<TestProcess> process
        = std::make_shared<TestProcess>(&controller);

    SECTION("Falls through if no processes") {
        REQUIRE(controller.getCurrentState()
            == ProcessControllerState::READY);

        controller.run();

        REQUIRE(controller.getCurrentState()
            == ProcessControllerState::STOPPED);
    }

    SECTION("Maintains state throughout run") {
        REQUIRE(controller.getCurrentState()
            == ProcessControllerState::READY);

        controller.addProcess(process, ProcessPrivilege::HIGH);
        controller.run();

        REQUIRE(controller.getCurrentState()
            == ProcessControllerState::STOPPED);

        REQUIRE(process->startPCState
            == ProcessControllerState::STARTING);
        REQUIRE(process->loopPCState
            == ProcessControllerState::RUNNING);
        REQUIRE(process->stopPCState
            == ProcessControllerState::STOPPING);
    }
}

TEST_CASE("Process_ProcessController_stop") {
    SECTION("Sets state to STOPPING") {
        ProcessController controller = ProcessController();

        REQUIRE(controller.getCurrentState()
            == ProcessControllerState::READY);
        controller.stop();
        REQUIRE(controller.getCurrentState()
            == ProcessControllerState::STOPPING);
    }
}

TEST_CASE("Process_ProcessController_kill") {
    SECTION("Sets state to KILLED") {
        ProcessController controller = ProcessController();

        REQUIRE(controller.getCurrentState() == ProcessControllerState::READY);
        controller.kill();
        REQUIRE(controller.getCurrentState() == ProcessControllerState::KILLED);
    }
}

TEST_CASE("Process_ProcessController_setFrameCap") {
    SECTION("Correctly sets frame time") {
        ProcessController controller = ProcessController();
        controller.setFrameCap(50);

        REQUIRE(controller.frameCap == 50);
        REQUIRE(controller.frameTime ==
            std::chrono::duration_cast<FrameClock::duration>(
                std::chrono::milliseconds(200)));
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

TEST_CASE("Process_ProcessController_sleepForRestOfFrame") {
    SECTION("Sleeps until wake time") {
        ProcessController controller = ProcessController();

        TestClock::setNextTimeStampToNow();
        TestClock::time_point frameStartTime = TestClock::now();
        unsigned int frameStartTimeCount =
            frameStartTime.time_since_epoch().count();

        controller.frameTime = std::chrono::duration_cast<TestClock::duration>(
            std::chrono::milliseconds(100));
        unsigned int frameTimeCount = controller.frameTime.count();

        controller.sleepForRestOfFrame(frameStartTime);

        TestClock::setNextTimeStampToNow();
        TestClock::time_point frameEndTime = TestClock::now();
        unsigned int frameEndTimeCount =
            frameEndTime.time_since_epoch().count();

        REQUIRE(frameEndTimeCount - frameStartTimeCount >= frameTimeCount);
    }
}

TEST_CASE("Process_ProcessController_shouldRunProcess") {
    ProcessController controller = ProcessController();
    std::shared_ptr<IProcess> process = std::make_shared<TestProcess>();
    auto instance = std::make_shared<ProcessInstance>(process);

    SECTION("Returns true if process and controller are low status") {
        controller.currentState = ProcessControllerState::READY;
        process->currentState = ProcessState::READY;

        REQUIRE(controller.shouldRunProcess(instance));
    }

    SECTION("Returns false if process is high status") {
        controller.currentState = ProcessControllerState::READY;
        process->currentState = ProcessState::REQUEST_KILL;

        REQUIRE_FALSE(controller.shouldRunProcess(instance));
    }

    SECTION("Returns false if controller is high status") {
        controller.currentState = ProcessControllerState::KILLED;
        process->currentState = ProcessState::READY;

        REQUIRE_FALSE(controller.shouldRunProcess(instance));
    }
}

TEST_CASE("Process_ProcessController_shouldContinueLoop") {
    ProcessController controller = ProcessController();
    std::shared_ptr<IProcess> process = std::make_shared<TestProcess>();

    SECTION("Returns true if processes and not stopped") {
        controller.currentState = ProcessControllerState::RUNNING;
        controller.addProcess(process);

        REQUIRE(controller.shouldContinueLoop());
    }

    SECTION("Returns false if controller is stopped") {
        controller.currentState = ProcessControllerState::STOPPING;
        controller.addProcess(process);

        REQUIRE_FALSE(controller.shouldContinueLoop());
    }

    SECTION("Returns false if no processes exist") {
        controller.currentState = ProcessControllerState::RUNNING;

        REQUIRE_FALSE(controller.shouldContinueLoop());
    }
}
