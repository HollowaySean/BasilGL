#include <catch.hpp>

#include <Basil/Process.hpp>

#include "ProcessTestUtils.hpp"

using basil::IProcess;
using basil::ProcessInstance;
using basil::ProcessPrivilege;
using basil::ProcessOrdinal;
using basil::ProcessState;

TEST_CASE("Process_ProcessInstance_ProcessInstance") {
    auto process = std::make_shared<TestProcess>();
    auto secondProcess = std::make_shared<TestProcess>();

    SECTION("Builds instance from existing process") {
        ProcessInstance firstInstance = ProcessInstance(process);

        std::string processName = "testProcess";
        secondProcess->setProcessName(processName);
        secondProcess->setCurrentState(ProcessState::REQUEST_STOP);
        ProcessInstance secondInstance = ProcessInstance(secondProcess);

        REQUIRE(firstInstance.processName ==
            ProcessInstance::DEFAULT_NAME +
                std::to_string(firstInstance.getID()));
        REQUIRE(secondInstance.processName == processName);

        REQUIRE(secondInstance.getID() == firstInstance.getID() + 1);

        REQUIRE(firstInstance.getCurrentState() == ProcessState::READY);
        REQUIRE(secondInstance.getCurrentState() == ProcessState::REQUEST_STOP);
    }
}

