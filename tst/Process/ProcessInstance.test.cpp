#include <catch.hpp>

#include "Process/ProcessInstance.hpp"

#include "Process/ProcessTestUtils.hpp"

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

        CHECK(firstInstance.processName ==
            ProcessInstance::DEFAULT_NAME +
                std::to_string(firstInstance.getID()));
        CHECK(secondInstance.processName == processName);

        CHECK(secondInstance.getID() == firstInstance.getID() + 1);

        CHECK(firstInstance.getCurrentState() == ProcessState::READY);
        CHECK(secondInstance.getCurrentState() == ProcessState::REQUEST_STOP);
    }
}

