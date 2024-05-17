#pragma once

#include "Process/IProcess.hpp"
#include "Process/ProcessController.hpp"
#include "Process/ProcessEnums.hpp"

using basil::IProcess;
using basil::ProcessController;
using basil::ProcessControllerState;
using basil::ProcessState;

class TestProcess : public IProcess {
 public:
    bool didStart = false;
    bool didLoop = false;
    bool didStop = false;

    ProcessControllerState startPCState = ProcessControllerState::READY;
    ProcessControllerState loopPCState = ProcessControllerState::READY;
    ProcessControllerState stopPCState = ProcessControllerState::READY;

    ProcessState stateAfterStart = ProcessState::READY;
    ProcessState stateAfterLoop = ProcessState::REQUEST_STOP;

    void onStart() override {
        IProcess::onStart();
        didStart = true;

        if (controller) {
            startPCState = controller->getCurrentState();
        }

        setCurrentState(stateAfterStart);
    }

    void onLoop() override {
        didLoop = true;

        if (controller) {
            loopPCState = controller->getCurrentState();
        }

        setCurrentState(stateAfterLoop);
    }

    void onStop() override {
        IProcess::onStop();
        didStop = true;

        if (controller) {
            stopPCState = controller->getCurrentState();
        }
    }
};


