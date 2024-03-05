#ifndef SRC_PROCESS_PROCESSSCHEDULE_HPP_
#define SRC_PROCESS_PROCESSSCHEDULE_HPP_

#include <list>
#include <memory>

#include "ProcessInstance.hpp"

namespace basil {

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
