#ifndef SRC_PROCESS_PROCESSCONTROLLER_HPP_
#define SRC_PROCESS_PROCESSCONTROLLER_HPP_

#include <functional>
#include <memory>

#include "ProcessEnums.hpp"
#include "ProcessSchedule.hpp"
#include "MetricsObserver.hpp"
#include "TimeSource.hpp"

// Include stubbed clock source for test build
// TODO(sholloway): Consolidate definitions like these
#ifdef TEST_BUILD
#include "Process/ChronoTestUtils.hpp"
using FrameClock = TestClock;
#else
using FrameClock = std::chrono::steady_clock;
#endif
using Timer = basil::TimeSource<FrameClock>;

namespace basil {

/** @brief Class which manages processes, keeps time, and records metrics. */
class ProcessController {
 public:
    /** @brief Create new process controller. */
    ProcessController();

    /** @brief Add process to main schedule.
     *  @param process      Pointer to IProcess to add
     *  @param privilege    Optional privilege level for process
    */
    void addProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege = std::nullopt);

    /** @brief Add process to early schedule.
     *  @param process      Pointer to IProcess to add
     *  @param privilege    Optional privilege level for process
    */
    void addEarlyProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege = std::nullopt);

    /** @brief Add process to late schedule.
     *  @param process      Pointer to IProcess to add
     *  @param privilege    Optional privilege level for process
    */
    void addLateProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege = std::nullopt);

    /** @brief Starts running processes. */
    void run();

    /** @brief Gracefully stops running. */
    void stop();

    /** @brief Stops running immediately. */
    void kill();

    /** @brief Current state enum of controller. */
    ProcessControllerState getCurrentState() { return currentState; }

    /** @brief Cap frame rate to specific value. */
    void setFrameCap(unsigned int framesPerSecond);

    /** @brief Get maximum frame rate. */
    unsigned int getFrameCap() { return frameCap; }

    /** @return Pointer to metrics observer. */
    std::shared_ptr<MetricsObserver> getMetricsObserver() { return metrics; }

#ifndef TEST_BUILD

 private:
#endif
    void sleepForRestOfFrame(FrameClock::time_point frameStartTime);

    void addProcessWithOrdinal(std::shared_ptr<IProcess> process,
        ProcessOrdinal ordinal,
        ProcessPrivilege privilege);
    void runProcessMethod(
        std::function<void(std::shared_ptr<IProcess>)> method);
    void interpretProcessState(
        std::shared_ptr<ProcessInstance> process);

    bool shouldRunProcess(
        std::shared_ptr<ProcessInstance> process);
    bool shouldContinueLoop();


    const std::function<void(std::shared_ptr<IProcess>)> startMethod
        = [](std::shared_ptr<IProcess> process) { process->onStart(); };
    const std::function<void(std::shared_ptr<IProcess>)> loopMethod
        = [](std::shared_ptr<IProcess> process) { process->onLoop(); };
    const std::function<void(std::shared_ptr<IProcess>)> stopMethod
        = [](std::shared_ptr<IProcess> process) { process->onStop(); };

    ProcessSchedule schedule;

    std::shared_ptr<MetricsObserver> metrics;

    ProcessControllerState currentState = ProcessControllerState::READY;
    unsigned int frameCap = 0;
    FrameClock::duration frameTime = std::chrono::seconds(0);
};

}  // namespace basil

#endif  // SRC_PROCESS_PROCESSCONTROLLER_HPP_
