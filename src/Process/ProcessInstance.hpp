#pragma once

#include <memory>
#include <string>

#include "Definitions.hpp"
#include "IProcess.hpp"
#include "ProcessEnums.hpp"

namespace basil {

/** @brief Struct containing metadata related to an instance of a process. */
struct ProcessInstance {
 public:
    /** @brief Create instance from IProcess */
    explicit ProcessInstance(std::shared_ptr<IProcess> process) {
        this->process = process;

        processID = NEXT_ID++;

        if (process->getProcessName().has_value()) {
            processName = process->getProcessName().value();
        } else {
            processName = DEFAULT_NAME + std::to_string(processID);
        }
    }

    /** @brief Shared pointer to frame process */
    std::shared_ptr<IProcess> process;

    /** @brief Human readable name of process */
    std::string processName;

    /** @brief Level of privilege for process
     *  @note  See ProcessPrivilege doc for more info */
    ProcessPrivilege privilegeLevel = DEFAULT_PRIVILEGE;

    /** @brief Loose ordering for process in schedule */
    ProcessOrdinal ordinal = DEFAULT_ORDINAL;

    /** @returns Unique ID of process instance */
    unsigned int getID() { return processID; }

    /** @returns Current state from IProcess */
    ProcessState getCurrentState() { return process->getCurrentState(); }

#ifndef TEST_BUILD

 private:
#endif
    unsigned int processID;

    inline static const ProcessOrdinal DEFAULT_ORDINAL
        = BASIL_DEFAULT_PROCESS_ORDINAL;
    inline static const ProcessPrivilege DEFAULT_PRIVILEGE
        = BASIL_DEFAULT_PROCESS_PRIVILEGE;
    inline static const std::string DEFAULT_NAME
        = BASIL_DEFAULT_PROCESS_NAME;

    inline static unsigned int NEXT_ID = 0;
};

}   // namespace basil
