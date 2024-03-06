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

namespace basil {

class ProcessController {
 public:
    ProcessController();

    void addProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege);
    void addEarlyProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege);
    void addLateProcess(
        std::shared_ptr<IProcess> process,
        std::optional<ProcessPrivilege> privilege);

    void run();
    void stop();
    void kill();

    ProcessControllerState getCurrentState() { return currentState; }

    void setFrameCap(unsigned int framesPerSecond);
    unsigned int getFrameCap() { return frameCap; }

#ifdef TEST_BUILD

 private:
#endif
    TimeSource<FrameClock> timeSource;

    void sleepForRestOfFrame(FrameClock::time_point frameStartTime);

    void addProcess(std::shared_ptr<IProcess> process,
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
    MetricsObserver metrics;

    ProcessControllerState currentState = ProcessControllerState::READY;
    unsigned int frameCap = 0;
    FrameClock::duration frameTime = std::chrono::seconds(0);
};

}  // namespace basil

#endif  // SRC_PROCESS_PROCESSCONTROLLER_HPP_