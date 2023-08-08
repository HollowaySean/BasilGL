#include "FrameController.hpp"
#include "DefaultTimerSource.hpp"

using FPState = IFrameProcess::State;

FrameController::FrameController(
        ITimerSource *newTimerSource) {
    setTimerSource(newTimerSource);
    frameCap = 0;
    currentState = READY;

    metrics = FrameMetrics();
    manager = ProcessManager(timerSource);
}

void FrameController::addProcess(IFrameProcess *processToAdd,
        Privilege privilegeLevel, std::string processName) {
    ProcessInstance *instance = new ProcessInstance {
        processToAdd,
        privilegeLevel,
        nextProcessID,
        processName
    };

    manager.addProcess(instance);
    nextProcessID++;
}

void FrameController::addProcess(IFrameProcess *processToAdd,
        std::string const processName) {
    addProcess(processToAdd, NONE, processName);
}

void FrameController::setFrameCap(int framesPerSecond) {
    frameCap = framesPerSecond;

    double frameTime = 1. / frameCap;
    timerSource->setMinimumFrameTime(frameTime);
}

void FrameController::setTimerSource(ITimerSource *newTimerSource) {
    if (!newTimerSource) {
        newTimerSource = new DefaultTimerSource();
    }
    timerSource = newTimerSource->clone();
}

void FrameController::run() {
    currentState = STARTING;

    manager.runStart();
    metrics.pushRecordFromTimer(timerSource);

    currentState = RUNNING;
    while (shouldRunLoop()) {
        manager.runLoop();
        if (manager.currentState > currentState) {
            currentState = manager.currentState;
        }

        timerSource->waitForFrameTime();
        metrics.pushRecordFromTimer(timerSource);
    }

    currentState = STOPPING;
    manager.runStop();
    metrics.pushRecordFromTimer(timerSource);

    currentState = STOPPED;
}

void FrameController::stop() {
    currentState = STOPPING;
}

void FrameController::kill() {
    currentState = KILLED;
}

bool FrameController::shouldRunLoop() {
    return currentState <= RUNNING &&
        manager.hasProcesses();
}

void FrameController::ProcessIterator::rectify() {
    if (currentPointer == early.end()) {
        currentPointer = main.begin();
        currentList = main;
    }

    if (currentPointer == main.end()) {
        currentPointer = late.begin();
        currentList = late;
    }
}

std::list<FrameController::ProcessInstance*>::iterator
FrameController::ProcessIterator::begin() {
    currentPointer = early.begin();
    currentList = early;

    rectify();
    return currentPointer;
}

FrameController::ProcessInstance*
FrameController::ProcessIterator::back() {
    if (late.size() > 0) {
        return late.back();
    }
    if (main.size() > 0) {
        return main.back();
    }
    return early.back();
}

std::list<FrameController::ProcessInstance*>::iterator
FrameController::ProcessIterator::end() {
    return late.end();
}

std::list<FrameController::ProcessInstance*>::iterator
FrameController::ProcessIterator::next() {
    ++currentPointer;
    rectify();
    return currentPointer;
}

int FrameController::ProcessIterator::size() {
    return early.size() + main.size() + late.size();
}

FrameController::ProcessManager::ProcessManager(
        std::shared_ptr<ITimerSource> newTimerSource):
            timerSource(newTimerSource) {}

void FrameController::ProcessManager::addEarlyProcess(
        ProcessInstance *newProcess) {
    if (newProcess) {
        processes.early.push_back(newProcess);
    }
}

void FrameController::ProcessManager::addProcess(
        ProcessInstance *newProcess) {
    if (newProcess) {
        processes.main.push_back(newProcess);
    }
}

void FrameController::ProcessManager::addLateProcess(
        ProcessInstance *newProcess) {
    if (newProcess) {
        processes.late.push_back(newProcess);
    }
}

void FrameController::ProcessManager::removeProcess(
        const ProcessInstance *processToRemove) {
    if (!processToRemove) {
        return;
    }

    std::list<ProcessInstance*>::iterator placeholder
        = processes.currentPointer;
    if (*(placeholder) == processToRemove) {
        std::list<ProcessInstance*> removeList
            = processes.currentList;
        processes.next();
        removeList.remove(*(placeholder));
    }

    std::list<ProcessInstance*>::iterator process
        = processes.begin();
    while (process != processes.end()) {
        if (*(process) == processToRemove) {
            processes.currentList.remove(*(process));
        }

        process = processes.next();
    }

    processes.currentPointer = placeholder;
    processes.rectify();
}

bool FrameController::ProcessManager::hasProcesses() {
    return processes.size() > 0;
}

void FrameController::ProcessManager::runStart() {
    runMethod(
        [](IFrameProcess *process) { process->onStart(); });
}

void FrameController::ProcessManager::runLoop() {
    runMethod(
        [](IFrameProcess *process) { process->onLoop(); });
}

void FrameController::ProcessManager::runStop() {
    runMethod(
        [](IFrameProcess *process) { process->onStop(); });
}

void FrameController::ProcessManager::runMethod(
        std::function<void(IFrameProcess*)> method) {
    timerSource->frameStart();
    currentState = RUNNING;

    std::list<ProcessInstance*>::iterator process
        = processes.begin();
    while (process != processes.end()) {
        int processID = (*process)->processID;

        FPState processState =
            (*process)->frameProcess->getCurrentState();
        Privilege privilege = (*process)->privilegeLevel;

        switch (processState) {
            case FPState::REQUEST_KILL:
                if (privilege >= Privilege::HIGH) {
                    currentState = KILLED;
                    return;
                }
            case FPState::REQUEST_STOP:
                if (privilege >= Privilege::LOW) {
                    currentState = STOPPING;
                }
                break;
            case FPState::REMOVE_PROCESS:
                removeProcess(*(process));
            case FPState::SKIP_PROCESS:
                process = processes.next();
                continue;
        }

        timerSource->processStart(processID);
        method((*process)->frameProcess);
        timerSource->processDone(processID);

        process = processes.next();
    }
    timerSource->frameDone();
}
