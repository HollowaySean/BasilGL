#include "Process/ProcessController.hpp"

namespace basil {

ProcessController::ProcessController()
    : schedule(), metrics() {}

std::shared_ptr<ProcessInstance>
ProcessController::addProcessWithOrdinal(std::shared_ptr<IProcess> process,
        ProcessOrdinal ordinal, ProcessPrivilege privilege) {
    process->onRegister(this);

    auto processInstance = std::make_shared<ProcessInstance>(process);

    processInstance->ordinal = ordinal;
    processInstance->privilegeLevel = privilege;

    schedule.addProcess(processInstance);
    return processInstance;

  /// Issue with coverage on this closing brace
}  // LCOV_EXCL_LINE

std::shared_ptr<ProcessInstance>
ProcessController::addProcess(std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege) {
    return addProcessWithOrdinal(process,
        ProcessOrdinal::MAIN,
        privilege.value_or(ProcessPrivilege::NONE));
}

std::shared_ptr<ProcessInstance>
ProcessController::addEarlyProcess(std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege) {
    return addProcessWithOrdinal(process,
        ProcessOrdinal::EARLY,
        privilege.value_or(ProcessPrivilege::NONE));
}

std::shared_ptr<ProcessInstance>
ProcessController::addLateProcess(std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege) {
    return addProcessWithOrdinal(process,
        ProcessOrdinal::LATE,
        privilege.value_or(ProcessPrivilege::NONE));
}

bool ProcessController::hasProcess(std::shared_ptr<IProcess> process) {
    for (auto instance = schedule.begin();
            instance != schedule.end();
            instance = schedule.next()) {
        if ((*instance)->process == process) {
            return true;
        }
    }

    return false;
}

bool ProcessController::hasProcess(const std::string_view& processName) {
    for (auto instance = schedule.begin();
            instance != schedule.end();
            instance = schedule.next()) {
        if ((*instance)->processName == processName) {
            return true;
        }
    }

    return false;
}

bool ProcessController::hasProcess(unsigned int processID) {
    for (auto instance = schedule.begin();
            instance != schedule.end();
            instance = schedule.next()) {
        if ((*instance)->getID() == processID) {
            return true;
        }
    }

    return false;
}

std::optional<std::shared_ptr<ProcessInstance>>
ProcessController::getProcess(const std::string_view& processName) {
    for (auto instance = schedule.begin();
            instance != schedule.end();
            instance = schedule.next()) {
        if ((*instance)->processName == processName) {
            return std::optional(*instance);
        }
    }

    return std::nullopt;
}

std::optional<std::shared_ptr<ProcessInstance>>
ProcessController::getProcess(unsigned int processID) {
    for (auto instance = schedule.begin();
            instance != schedule.end();
            instance = schedule.next()) {
        if ((*instance)->getID() == processID) {
            return std::optional(*instance);
        }
    }

    return std::nullopt;
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

    logger.log("Stopping loop", LogLevel::INFO);
}

void ProcessController::kill() {
    currentState = ProcessControllerState::KILLED;
}

void ProcessController::setFrameCap(unsigned int framesPerSecond) {
    frameCap = framesPerSecond;
    frameTime = FrameTimer::frequencyToPeriod(framesPerSecond);
}

void ProcessController::runProcessMethod(
        std::function<void(std::shared_ptr<IProcess>)> method) {
    auto frameStartTime = FrameTimer::getTimestamp();
    metrics.recordFrameStart(frameStartTime);

    for (auto instance = schedule.begin();
            instance != schedule.end();
            instance = schedule.next()) {
        if (shouldRunProcess(*instance)) {
            auto processStartTime = FrameTimer::getTimestamp();
            method((*instance)->process);
            auto processStopTime = FrameTimer::getTimestamp();

            auto processDuration = processStopTime - processStartTime;
            metrics.recordProcessTime(*instance, processDuration);
        }

        interpretProcessState(*instance);
    }

    auto frameStopTime = FrameTimer::getTimestamp();
    metrics.recordWorkEnd(frameStopTime);

    sleepForRestOfFrame(frameStartTime);

    auto wakeTime = FrameTimer::getTimestamp();
    metrics.recordFrameEnd(wakeTime);
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
    FrameTimer::waitUntilTime(frameWakeTime);
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

ProcessController::Builder&
ProcessController::Builder::withFrameCap(unsigned int framesPerSecond) {
    impl->setFrameCap(framesPerSecond);
    return *this;
}

ProcessController::Builder&
ProcessController::Builder::withProcess(std::shared_ptr<IProcess> process,
        ProcessPrivilege privilege) {
    impl->addProcess(process, privilege);
    return *this;
}

ProcessController::Builder&
ProcessController::Builder::withEarlyProcess(std::shared_ptr<IProcess> process,
        ProcessPrivilege privilege) {
    impl->addEarlyProcess(process, privilege);
    return *this;
}

ProcessController::Builder&
ProcessController::Builder::withLateProcess(std::shared_ptr<IProcess> process,
        ProcessPrivilege privilege) {
    impl->addLateProcess(process, privilege);
    return *this;
}

}  // namespace basil
