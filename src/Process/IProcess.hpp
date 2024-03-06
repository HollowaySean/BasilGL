#ifndef SRC_PROCESS_IPROCESS_HPP_
#define SRC_PROCESS_IPROCESS_HPP_

#include <optional>
#include <string>

#include "ProcessEnums.hpp"

namespace basil {

/** @brief Interface consumable by ProcessController.
 *  Requires defining 'onLoop' function to run during frame.
*/
class IProcess {
 public:
    /** @return Current state of process. */
    virtual ProcessState getCurrentState() const {
        return currentState;
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

    /** @brief Set default name for process instance. */
    void setProcessName(const std::string& processName) {
        this->processName = processName;
    }

#ifndef TEST_BUILD

 protected:
#endif
    /** @brief Sets state of process. */
    virtual void setCurrentState(ProcessState newState) {
        newState = currentState;
    }

#ifndef TEST_BUILD

 private:
#endif
    ProcessState currentState = ProcessState::READY;
    std::optional<std::string> processName = std::nullopt;
};

}  // namespace basil

#endif  // SRC_PROCESS_IPROCESS_HPP_
