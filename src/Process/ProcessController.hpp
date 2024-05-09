#ifndef SRC_PROCESS_PROCESSCONTROLLER_HPP_
#define SRC_PROCESS_PROCESSCONTROLLER_HPP_

#include <functional>
#include <memory>

#include "FrameClock.hpp"
#include "IProcess.hpp"
#include "ProcessEnums.hpp"
#include "ProcessInstance.hpp"
#include "ProcessSchedule.hpp"
#include "MetricsObserver.hpp"

namespace basil {

/** @brief Class which manages processes, keeps time, and records metrics. */
class ProcessController : private IBuildable<ProcessController> {
 public:
    /** @brief Create new process controller. */
    ProcessController();

    /** @brief   Add process to main schedule.
     *  @param   process      Pointer to IProcess to add
     *  @param   privilege    Optional privilege level for process
     *  @returns ProcessInstance wrapper
    */
    std::shared_ptr<ProcessInstance> addProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege = std::nullopt);

    /** @brief Add process to early schedule.
     *  @param process      Pointer to IProcess to add
     *  @param privilege    Optional privilege level for process
     *  @returns ProcessInstance wrapper
    */
    std::shared_ptr<ProcessInstance> addEarlyProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege = std::nullopt);

    /** @brief Add process to late schedule.
     *  @param process      Pointer to IProcess to add
     *  @param privilege    Optional privilege level for process
     *  @returns ProcessInstance wrapper
    */
    std::shared_ptr<ProcessInstance> addLateProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege = std::nullopt);

    /** @brief Add process with given parameters.
     *  @param process      Pointer to IProcess to add
     *  @param ordinal      Ordinal indicating process timing
     *  @param privilege    Privilege level for process
     *  @returns ProcessInstance wrapper
    */
    std::shared_ptr<ProcessInstance>
    addProcessWithOrdinal(std::shared_ptr<IProcess> process,
        ProcessOrdinal ordinal,
        ProcessPrivilege privilege);

    /** @param process      Pointer to IProcess to check for
     *  @returns Boolean indicating if process exists in schedule */
    bool hasProcess(std::shared_ptr<IProcess> process);

    /** @param processName  Name of process instance to check for
     *  @returns Boolean indicating if process exists in schedule */
    bool hasProcess(const std::string_view& processName);

    /** @param processID    UID of process instance to check for
     *  @returns Boolean indicating if process exists in schedule */
    bool hasProcess(unsigned int processID);

    /** @param processName  Name of process instance to check for
     *  @returns Pointer to first process instance with name */
    std::optional<std::shared_ptr<ProcessInstance>>
    getProcess(const std::string_view& processName);

    /** @param processID    UID of process instance to check for
     *  @returns Pointer to process instance with UID */
    std::optional<std::shared_ptr<ProcessInstance>>
    getProcess(unsigned int processID);

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
    MetricsObserver& getMetricsObserver() { return metrics; }

    /** @brief Builder pattern for ProcessController. */
    class Builder : public IBuilder<ProcessController> {
     public:
        /** @brief Set maximum frame rate. */
        Builder& withFrameCap(unsigned int framesPerSecond);

        /** @brief Set process to run in main loop. */
        Builder& withProcess(std::shared_ptr<IProcess> process,
            ProcessPrivilege privilege = ProcessPrivilege::NONE);

        /** @brief Set process to run early. */
        Builder& withEarlyProcess(std::shared_ptr<IProcess> process,
            ProcessPrivilege privilege = ProcessPrivilege::NONE);

        /** @brief Set process to run late. */
        Builder& withLateProcess(std::shared_ptr<IProcess> process,
            ProcessPrivilege privilege = ProcessPrivilege::NONE);
    };

#ifndef TEST_BUILD

 private:
#endif
    void sleepForRestOfFrame(FrameClock::time_point frameStartTime);
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
    MetricsObserver metrics;

    ProcessControllerState currentState = ProcessControllerState::READY;
    unsigned int frameCap = 0;
    FrameClock::duration frameTime = std::chrono::seconds(0);
};

}  // namespace basil

#endif  // SRC_PROCESS_PROCESSCONTROLLER_HPP_
