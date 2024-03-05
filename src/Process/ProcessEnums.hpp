#ifndef SRC_PROCESS_PROCESSENUMS_HPP_
#define SRC_PROCESS_PROCESSENUMS_HPP_

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

}  // namespace basil

#endif  // SRC_PROCESS_PROCESSENUMS_HPP_
