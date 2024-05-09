#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::IProcess;
using basil::ProcessState;

TEST_CASE("Process_IProcess_onRegister") {
    SECTION("Saves pointer to controller") {
        auto process = std::make_shared<TestProcess>();
        ProcessController controller = ProcessController();

        CHECK(process->controller == nullptr);
        controller.addProcess(process);
        CHECK(process->controller == &controller);
    }
}

TEST_CASE("Process_IProcess_onStart") {
    SECTION("Runs overriden start method") {
        TestProcess process = TestProcess();

        CHECK_FALSE(process.didStart);
        process.onStart();
        CHECK(process.didStart);
    }
}

TEST_CASE("Process_IProcess_onLoop") {
    SECTION("Runs overriden loop method") {
        TestProcess process = TestProcess();

        CHECK_FALSE(process.didLoop);
        process.onLoop();
        CHECK(process.didLoop);
    }
}

TEST_CASE("Process_IProcess_onStop") {
    SECTION("Runs overriden stop method") {
        TestProcess process = TestProcess();

        CHECK_FALSE(process.didStop);
        process.onStop();
        CHECK(process.didStop);
    }
}

TEST_CASE("Process_IProcess_setCurrentState") {
    SECTION("Sets state enum") {
        TestProcess process = TestProcess();

        CHECK(process.getCurrentState() == ProcessState::READY);
        process.setCurrentState(ProcessState::REQUEST_STOP);
        CHECK(process.getCurrentState() == ProcessState::REQUEST_STOP);
    }
}
