#pragma once

#include <optional>
#include <string>

#include "ProcessEnums.hpp"

namespace basil {

class ProcessController;

/** @brief Interface consumable by ProcessController.
 *  Requires defining 'onLoop' function to run during frame.
*/
class IProcess {
 public:
    /** @brief Create implementation object of interface. */
    IProcess() : controller(nullptr) {}

    /** @return Current state of process. */
    ProcessState getCurrentState() const {
        return currentState;
    }

    /** @brief Function to run process is scheduled. */
    virtual void onRegister(ProcessController* controller) {
        this->controller = controller;
    }

    /** @brief Function to run before first frame. */
    virtual void onStart() {}

    /** @brief Function to run repeatedly within successive frames. */
    virtual void onLoop() = 0;

    /** @brief Function to run after last frame. */
    virtual void onStop() {}

    /** @return Name of process instance. */
    std::optional<std::string> getProcessName() {
        return processName;
    }

    /** @return Pointer to controller that process is registered to. */
    ProcessController* getController() { return controller; }

#ifndef TEST_BUILD

 protected:
#endif
    /** @brief Set default name for process instance. */
    void setProcessName(const std::string& processName) {
        this->processName = processName;
    }

    /** @brief Sets state of process. */
    void setCurrentState(ProcessState newState) {
        currentState = newState;
    }

    /** @brief Pointer to controller running process. */
    ProcessController* controller;

#ifndef TEST_BUILD

 private:
#endif
    ProcessState currentState = ProcessState::READY;
    std::optional<std::string> processName = std::nullopt;
};

}   // namespace basil
