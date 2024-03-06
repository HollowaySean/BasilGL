#ifndef SRC_PROCESS_PROCESSENUMS_HPP_
#define SRC_PROCESS_PROCESSENUMS_HPP_

namespace basil {

/** @brief Enum type representing privilege levels for a given process.
 *  NONE - Can run, pause itself, and remove itself
 *  LOW  - Can stop controller
 *  HIGH - Can kill controller
*/
enum class ProcessPrivilege {
    NONE, LOW, HIGH
};

/** @brief Enum type representing loose ordering for processes.
 *  Possible values are EARLY, MAIN, and LATE.
 */
enum class ProcessOrdinal {
    EARLY, MAIN, LATE
};

/** @brief Enum type representing the state of a process.
 *  Ordered by severity of status.
*/
enum class ProcessState {
    READY, REQUEST_STOP, SKIP_PROCESS, REMOVE_PROCESS, REQUEST_KILL
};

/** @brief Enum type representing state of ProcessController
 *  Ordered by severity of status.
*/
enum class ProcessControllerState {
    READY, STARTING, RUNNING, STOPPING, STOPPED, KILLED
};

}  // namespace basil

#endif  // SRC_PROCESS_PROCESSENUMS_HPP_
