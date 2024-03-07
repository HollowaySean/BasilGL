#include "ProcessController.hpp"

namespace basil {

ProcessController::ProcessController() : schedule(), metrics(), timeSource() {}

void ProcessController::addProcessWithOrdinal(std::shared_ptr<IProcess> process,
        ProcessOrdinal ordinal, ProcessPrivilege privilege) {
    auto processInstance = std::make_shared<ProcessInstance>(process);

    processInstance->ordinal = ordinal;
    processInstance->privilegeLevel = privilege;

    schedule.addProcess(processInstance);
}

void ProcessController::addProcess(std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege) {
    addProcessWithOrdinal(process,
        ProcessOrdinal::MAIN,
        privilege.value_or(ProcessPrivilege::NONE));
}

void ProcessController::addEarlyProcess(std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege) {
    addProcessWithOrdinal(process,
        ProcessOrdinal::EARLY,
        privilege.value_or(ProcessPrivilege::NONE));
}

void ProcessController::addLateProcess(std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege) {
    addProcessWithOrdinal(process,
        ProcessOrdinal::LATE,
        privilege.value_or(ProcessPrivilege::NONE));
}

void ProcessController::run() {
    currentState = ProcessControllerState::STARTING;
    runProcessMethod(startMethod);

    if (currentState == ProcessControllerState::KILLED) return;

    currentState = ProcessControllerState::RUNNING;
    while (shouldContinueLoop()) {
        runProcessMethod(loopMethod);
    }

    if (currentState == ProcessControllerState::KILLED) return;

    currentState = ProcessControllerState::STOPPING;
    runProcessMethod(stopMethod);
    currentState = ProcessControllerState::STOPPED;
}

void ProcessController::stop() {
    currentState = ProcessControllerState::STOPPING;
}

void ProcessController::kill() {
    currentState = ProcessControllerState::KILLED;
}

void ProcessController::setFrameCap(unsigned int framesPerSecond) {
    frameCap = framesPerSecond;
    frameTime = timeSource.frequencyToPeriod(framesPerSecond);
}

void ProcessController::runProcessMethod(
        std::function<void(std::shared_ptr<IProcess>)> method) {
    auto frameStartTime = timeSource.getTimestamp();

    for (auto instance = schedule.begin();
            instance != schedule.end();
            instance = schedule.next()) {
        if (shouldRunProcess(*instance)) {
            auto processStartTime = timeSource.getTimestamp();
            method((*instance)->process);
            auto processStopTime = timeSource.getTimestamp();

            auto processDuration = processStopTime - processStartTime;
            metrics.recordProcessTime(*instance, processDuration);
        }

        interpretProcessState(*instance);
    }

    auto frameStopTime = timeSource.getTimestamp();
    auto frameDuration = frameStopTime - frameStartTime;
    metrics.recordFrameTime(frameDuration);

    sleepForRestOfFrame(frameStartTime);
}

void ProcessController::interpretProcessState(
        std::shared_ptr<ProcessInstance> process) {
    ProcessState state = process->getCurrentState();
    ProcessPrivilege privilege = process->privilegeLevel;

    switch (state) {
        case ProcessState::REQUEST_KILL:
            if (privilege >= ProcessPrivilege::HIGH) {
                currentState = ProcessControllerState::KILLED;
                return;
            }
            // Falls through to request stop if privilege is not met

        case ProcessState::REQUEST_STOP:
            if (privilege >= ProcessPrivilege::LOW) {
                currentState = ProcessControllerState::STOPPING;
            }
            return;

        case ProcessState::REMOVE_PROCESS:
            schedule.removeProcess(process);
            return;
    }
}

void ProcessController::sleepForRestOfFrame(
        FrameClock::time_point frameStartTime) {
    auto frameWakeTime = frameStartTime + frameTime;
    timeSource.waitUntilTime(frameWakeTime);
}

bool ProcessController::shouldRunProcess(
        std::shared_ptr<ProcessInstance> process) {
    ProcessState state = process->getCurrentState();

    bool processShouldRun = state < ProcessState::SKIP_PROCESS;
    bool controllerShouldRun = currentState < ProcessControllerState::STOPPED;

    return processShouldRun && controllerShouldRun;
}

bool ProcessController::shouldContinueLoop() {
    return schedule.size() > 0 &&
        currentState < ProcessControllerState::STOPPING;
}

}  // namespace basil
