#include "ProcessSchedule.hpp"

namespace basil {

ProcessInstance::ProcessInstance(const IFrameProcess& frameProcess)
        : frameProcess(frameProcess) {
    processID = NEXT_ID++;
    processName = DEFAULT_NAME + std::to_string(processID);
}

void ProcessSchedule::addProcess(std::shared_ptr<ProcessInstance> newProcess) {
    if (!newProcess) return;

    std::list<std::shared_ptr<ProcessInstance>> listToAppend;
    switch (newProcess->ordinal) {
        case ProcessOrdinal::EARLY:
            listToAppend = early;
            break;
        case ProcessOrdinal::LATE:
            listToAppend = late;
            break;
        default:
            listToAppend = main;
    }

    listToAppend.push_back(newProcess);
}

void ProcessSchedule::removeProcess(
        std::shared_ptr<ProcessInstance> processToRemove) {
    if (!processToRemove) return;

    for (auto process = begin(); process != end(); process = next()) {
        if (*process == processToRemove) {
            auto listToRemoveFrom = currentList;

            // Safeguard in case currently pointing to process to remove
            if (process == currentInstance) {
                next();
            }

            listToRemoveFrom.remove(processToRemove);
        }
    }
}

std::shared_ptr<ProcessInstance> ProcessSchedule::front() {
    return *(begin());
}

std::shared_ptr<ProcessInstance> ProcessSchedule::back() {
    if (late.size()) return late.back();

    if (main.size()) return main.back();

    return early.back();
}

std::list<std::shared_ptr<ProcessInstance>>::const_iterator
ProcessSchedule::begin() {
    currentInstance = early.begin();
    currentList = early;

    rectifySchedule();
    return currentInstance;
}

std::list<std::shared_ptr<ProcessInstance>>::const_iterator
ProcessSchedule::end() {
    return late.end();
}

std::list<std::shared_ptr<ProcessInstance>>::const_iterator
ProcessSchedule::next() {
    ++currentInstance;
    rectifySchedule();
    return currentInstance;
}

unsigned int ProcessSchedule::size() {
    return early.size() + main.size() + late.size();
}

void ProcessSchedule::rectifySchedule() {
    if (currentInstance == early.end()) {
        currentInstance = main.begin();
        currentList = main;
    }

    if (currentInstance == main.end()) {
        currentInstance = late.begin();
        currentList = late;
    }
}

}  // namespace basil
