#ifndef SRC_PROCESS_PROCESSINSTANCE_HPP_
#define SRC_PROCESS_PROCESSINSTANCE_HPP_

#include <string>

#include <Basil/Frame.hpp>

namespace basil {

/** @brief Enum type representing privilege levels for a given process.
 *  NONE - Can run, pause itself, and remove itself
 *  LOW  - Can stop controller
 *  HIGH - Can kill controller
*/
enum class ProcessPrivilege { NONE, LOW, HIGH };

/** @brief Enum type representing loose ordering for processes.
 *  Possible values are EARLY, MAIN, and LATE.
 */
enum class ProcessOrdinal { EARLY, MAIN, LATE };

// TODO(sholloway): Documentation
struct ProcessInstance {
 public:
    explicit ProcessInstance(const IFrameProcess& frameProcess)
            : frameProcess(frameProcess) {
        processID = NEXT_ID++;
        processName = DEFAULT_NAME + std::to_string(processID);
    }

    const IFrameProcess& frameProcess;

    std::string processName;

    ProcessPrivilege privilegeLevel = DEFAULT_PRIVILEGE;
    ProcessOrdinal ordinal = DEFAULT_ORDINAL;

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
