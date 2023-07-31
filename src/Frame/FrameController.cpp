#include "FrameController.hpp"
#include "DefaultTimerSource.hpp"

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

void FrameController::run() {
    currentState = STARTING;

    manager.runStart();

    currentState = RUNNING;
    while (shouldRunLoop()) {
        timerSource->frameStart();
        manager.runLoop();
        timerSource->frameDone();

        timerSource->waitForFrameTime();

        TimerRecord currentRecord = timerSource->getRecord();
        metrics.pushTimerRecord(currentRecord);
    }

    currentState = STOPPING;
    manager.runStop();

    currentState = STOPPED;
}

void FrameController::stop() {
    currentState = STOPPING;
}

void FrameController::kill() {
    currentState = KILLED;
}

bool FrameController::shouldRunLoop() {
    return currentState == RUNNING &&
        manager.hasProcesses();
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
