#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::IProcess;
using basil::ProcessState;

TEST_CASE("Process_IProcess_onStart") {
    SECTION("Runs overriden start method") {
        TestProcess process = TestProcess();

        REQUIRE_FALSE(process.didStart);
        process.onStart();
        REQUIRE(process.didStart);
    }
}

TEST_CASE("Process_IProcess_onLoop") {
    SECTION("Runs overriden loop method") {
        TestProcess process = TestProcess();

        REQUIRE_FALSE(process.didLoop);
        process.onLoop();
        REQUIRE(process.didLoop);
    }
}

TEST_CASE("Process_IProcess_onStop") {
    SECTION("Runs overriden stop method") {
        TestProcess process = TestProcess();

        REQUIRE_FALSE(process.didStop);
        process.onStop();
        REQUIRE(process.didStop);
    }
}

TEST_CASE("Process_IProcess_setCurrentState") {
    SECTION("Sets state enum") {
        TestProcess process = TestProcess();

        REQUIRE(process.getCurrentState() == ProcessState::READY);
        process.setCurrentState(ProcessState::REQUEST_STOP);
        REQUIRE(process.getCurrentState() == ProcessState::REQUEST_STOP);
    }
}
