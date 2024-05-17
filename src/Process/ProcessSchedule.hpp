#pragma once

#include <list>
#include <memory>

#include "ProcessInstance.hpp"

namespace basil {

/** @brief Class which maintains schedule of processes in early, main, and late
 *      process lists. */
class ProcessSchedule {
 public:
    /** @brief Adds process to schedule */
    void addProcess(std::shared_ptr<ProcessInstance> newProcess);

    /** @brief Removes process if present in schedule */
    void removeProcess(std::shared_ptr<ProcessInstance> processToRemove);

    /** @brief Returns earliest process in schedule */
    std::shared_ptr<ProcessInstance> front();

    /** @brief Returns latest process in schedule */
    std::shared_ptr<ProcessInstance> back();

    /** @brief Starting iterator for schedule */
    std::list<std::shared_ptr<ProcessInstance>>
        ::const_iterator begin();

    /** @brief Ending iterator for schedule */
    std::list<std::shared_ptr<ProcessInstance>>
        ::const_iterator end();

    /** @brief Increment iterator */
    std::list<std::shared_ptr<ProcessInstance>>
        ::const_iterator next();

    unsigned int size();

#ifndef TEST_BUILD

 private:
#endif
    std::list<std::shared_ptr<ProcessInstance>> early;
    std::list<std::shared_ptr<ProcessInstance>> main;
    std::list<std::shared_ptr<ProcessInstance>> late;

    std::list<std::shared_ptr<ProcessInstance>>
        ::const_iterator currentInstance;
    std::list<std::shared_ptr<ProcessInstance>>* currentList
        = &early;

    void rectifySchedule();
};

}   // namespace basil
