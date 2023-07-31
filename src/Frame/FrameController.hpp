#ifndef SRC_FRAME_FRAMECONTROLLER_HPP_
#define SRC_FRAME_FRAMECONTROLLER_HPP_

#include <functional>
#include <list>
#include <memory>
#include <string>

#include "FrameMetrics.hpp"
#include "IFrameProcess.hpp"
#include "ITimerSource.hpp"

/**
 * @brief Utility which manages list of IFrameProcess objects, runs
 * their start, loop, and stop functions, and manages timing metrics.
 */
class FrameController {
 public:
    /** @brief Creates new FrameController with optional argument of ITimerSource,
     *  which otherwise creates new DefaultTimerSource. */
    explicit FrameController(ITimerSource *timerSource = nullptr);


    // Internal Types
    /** @brief Enum type representing possible states of the controller. */
    enum FCState {
        READY, STARTING, RUNNING, PAUSED, STOPPING, STOPPED, KILLED
    };

    /** @brief Enum type representing privilege levels for a given process.
     *  NONE - Can run, pause itself, and remove itself
     *  LOW  - Can stop controller
     *  HIGH - Can kill controller
    */
    enum Privilege { NONE, LOW, HIGH };


    // Process Management
    /** @brief Adds a process
     *  @param processToAdd     Pointer to process to add
     *  @param privilegeLevel   [Optional] Admin privilege level for process
     *  @param processName      [Optional] Human readable process name
    */
    void addProcess(IFrameProcess *processToAdd,
            Privilege privilegeLevel = NONE,
            std::string processName = "anonymous");
    void addProcess(IFrameProcess *processToAdd,
            std::string processName);


    // Control flow
    /** @brief Begins frame processes. */
    void start();

    /** @brief Stops loop after all processes complete. */
    void stop();

    /** @brief Stops loop immediately. */
    void kill();


    // Setters
    /** @param framesPerSecond Maximum frame rate.
     *  Uncapped frame rate when set to 0. */
    void setFrameCap(int framesPerSecond);

    /** @param newTimerSource Pointer to instance of ITimerSource to use. */
    void setTimerSource(ITimerSource *newTimerSource);


    // Getters
    /** @return Maximum frame rate cap. Uncapped frame rate if set to 0. */
    int getFrameCap() { return frameCap; }

    /** @returns The current state of the controller. */
    FCState getCurrentState() { return currentState; }

#ifndef TEST_BUILD

 private:
#endif
    // Main methods
    void runProcesses();

    // Internal Types
    struct ProcessInstance {
        IFrameProcess *frameProcess;
        Privilege privilegeLevel = NONE;
        int processID = -1;
        std::string processName = "anonymous";
    };

    class ProcessManager {
     public:
        friend class FrameController;

    #ifndef TEST_BUILD

     private:
    #endif
        // Temporary - Will add more detail
        explicit ProcessManager(
            std::shared_ptr<ITimerSource> timerSource = nullptr);
        std::list<ProcessInstance*> processes;
        std::shared_ptr<ITimerSource> timerSource;


        void addProcess(ProcessInstance *newProcess);
        // TODO(sholloway): Requires smarter container than a list
        // void removeProcess(ProcessInstance *processToRemove);

        void runStart();
        void runLoop();
        void runStop();
        void runMethod(std::function<void(IFrameProcess*)> method);
    };
    ProcessManager manager;

    // State
    FCState currentState;
    int nextProcessID = 0;

    // Timing
    std::shared_ptr<ITimerSource> timerSource;
    int frameCap;
};

#endif  // SRC_FRAME_FRAMECONTROLLER_HPP_
