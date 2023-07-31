#include "FrameController.hpp"
#include "DefaultTimerSource.hpp"

FrameController::FrameController(
        ITimerSource *newTimerSource) {
    setTimerSource(newTimerSource);
    frameCap = 0;
    currentState = READY;

    manager = ProcessManager(timerSource);
}

void FrameController::addProcess(IFrameProcess *processToAdd,
        Privilege privilegeLevel, std::string processName) {
    ProcessInstance *instance = new ProcessInstance {
        processToAdd,
        privilegeLevel,
        nextProcessID++,
        processName
    };

    manager.addProcess(instance);
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

void FrameController::start() {
    runProcesses();
}

void FrameController::stop() {
    currentState = STOPPING;
}

void FrameController::kill() {
    currentState = KILLED;
}

void FrameController::runProcesses() {
    // TODO(sholloway): Sub out for a more comprehensive state check
    // currentState = STARTING;

    manager.runStart();

    // TODO(sholloway): Sub out for a more comprehensive state check
    // currentState = RUNNING;
    while (currentState == RUNNING) {
        timerSource->frameStart();
        manager.runLoop();
        timerSource->frameDone();

        timerSource->waitForFrameTime();
    }

    // TODO(sholloway): Sub out for a more comprehensive state check
    // currentState = STOPPING;
    manager.runStop();

    // currentState = STOPPED;
}

FrameController::ProcessManager::ProcessManager(
        std::shared_ptr<ITimerSource> newTimerSource):
            timerSource(newTimerSource) {}

void FrameController::ProcessManager::addProcess(
        ProcessInstance *newProcess) {
    if (newProcess) {
        processes.push_back(newProcess);
    }
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
    std::list<ProcessInstance*>::iterator process;
    for (   process = processes.begin();
            process != processes.end();
            ++process) {
        int processID = (*process)->processID;

        timerSource->processStart(processID);
        method((*process)->frameProcess);
        timerSource->processDone(processID);
    }
}
