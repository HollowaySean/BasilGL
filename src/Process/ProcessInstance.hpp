#ifndef SRC_PROCESS_PROCESSINSTANCE_HPP_
#define SRC_PROCESS_PROCESSINSTANCE_HPP_

#include <string>

#include <Basil/Frame.hpp>

#include "ProcessEnums.hpp"

namespace basil {

/** @brief Struct containing metadata related to an instance of a process. */
struct ProcessInstance {
 public:
    /** @brief Create instance from IFrameProcess */
    explicit ProcessInstance(const IFrameProcess& frameProcess)
            : frameProcess(frameProcess) {
        processID = NEXT_ID++;
        processName = DEFAULT_NAME + std::to_string(processID);
    }

    /** @brief Const reference to frame process */
    const IFrameProcess& frameProcess;

    /** @brief Human readable name of process */
    std::string processName;

    /** @brief Level of privilege for process
     *  @note  See ProcessPrivilege doc for more info */
    ProcessPrivilege privilegeLevel = DEFAULT_PRIVILEGE;

    /** @brief Loose ordering for process in schedule */
    ProcessOrdinal ordinal = DEFAULT_ORDINAL;

    /** @returns Unique ID of process instance */
    unsigned int getID() { return processID; }

#ifndef TEST_BUILD

 private:
#endif
    unsigned int processID;

    inline static const ProcessOrdinal DEFAULT_ORDINAL
        = ProcessOrdinal::MAIN;
    inline static const ProcessPrivilege DEFAULT_PRIVILEGE
        = ProcessPrivilege::NONE;
    inline static const std::string DEFAULT_NAME
        = "anonymous";
    inline static unsigned int NEXT_ID = 0;
};

}  // namespace basil

#endif  // SRC_PROCESS_PROCESSINSTANCE_HPP_
