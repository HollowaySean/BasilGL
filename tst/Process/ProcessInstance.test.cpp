#include <catch.hpp>

#include <Basil/Frame.hpp>
#include <Basil/Process.hpp>

using basil::IFrameProcess;
using basil::ProcessInstance;
using basil::ProcessPrivilege;
using basil::ProcessOrdinal;

class TestProcess : public IFrameProcess {
    void onLoop() override {}
};

TEST_CASE("Process_ProcessInstance_ProcessInstance") {
    TestProcess process = TestProcess();

    SECTION("Increments process ID") {
        ProcessInstance firstInstance = ProcessInstance(process);
        ProcessInstance secondInstance = ProcessInstance(process);

        REQUIRE(secondInstance.getID() == firstInstance.getID() + 1);

        REQUIRE(firstInstance.processName ==
            ProcessInstance::DEFAULT_NAME +
                std::to_string(firstInstance.getID()));
        REQUIRE(secondInstance.processName ==
            ProcessInstance::DEFAULT_NAME +
                std::to_string(secondInstance.getID()));
    }
}

