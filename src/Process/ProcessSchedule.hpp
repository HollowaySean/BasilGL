#ifndef SRC_PROCESS_PROCESSSCHEDULE_HPP_
#define SRC_PROCESS_PROCESSSCHEDULE_HPP_

#include <list>
#include <memory>
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
    explicit ProcessInstance(const IFrameProcess& frameProcess);

    const IFrameProcess& frameProcess;

    ProcessPrivilege privilegeLevel = DEFAULT_PRIVILEGE;
    ProcessOrdinal ordinal = DEFAULT_ORDINAL;
    std::string processName;

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

// TODO(sholloway): Documentation
class ProcessSchedule {
 public:
    void addProcess(std::shared_ptr<ProcessInstance> newProcess);
    void removeProcess(std::shared_ptr<ProcessInstance> processToRemove);

    std::shared_ptr<ProcessInstance> front();
    std::shared_ptr<ProcessInstance> back();

    std::list<std::shared_ptr<ProcessInstance>>
        ::const_iterator begin();
    std::list<std::shared_ptr<ProcessInstance>>
        ::const_iterator end();
    std::list<std::shared_ptr<ProcessInstance>>
        ::const_iterator next();

    unsigned int size();

 private:
    std::list<std::shared_ptr<ProcessInstance>> early;
    std::list<std::shared_ptr<ProcessInstance>> main;
    std::list<std::shared_ptr<ProcessInstance>> late;

    std::list<std::shared_ptr<ProcessInstance>>
        ::const_iterator currentInstance;
    std::list<std::shared_ptr<ProcessInstance>> currentList;

    void rectifySchedule();
};

}  // namespace basil

#endif  // SRC_PROCESS_PROCESSSCHEDULE_HPP_
