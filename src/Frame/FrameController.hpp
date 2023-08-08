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
        READY, STARTING, RUNNING, STOPPING, STOPPED, KILLED
    };

    /** @brief Enum type representing privilege levels for a given process.
     *  NONE - Can run, pause itself, and remove itself
     *  LOW  - Can stop controller
     *  HIGH - Can kill controller
    */
    enum Privilege { NONE, LOW, HIGH };

    /** @brief Enum type representing loose ordering for processes.
     *  Possible values are EARLY, MAIN, and LATE.
     */
    enum Ordinal { EARLY, MAIN, LATE };


    // Process Management
    /** @brief Adds a process
     *  @param processToAdd     Pointer to process to add
     *  @param privilegeLevel   [Optional] Admin privilege level for process
     *  @param processName      [Optional] Human readable process name
     *  @param ordinal          [Optional] Enum indicating whether to run early or late
    */
    void addProcess(IFrameProcess *processToAdd,
            Privilege privilegeLevel = NONE,
            std::string processName = "anonymous",
            Ordinal ordinal = MAIN);
    void addProcess(IFrameProcess *processToAdd,
            std::string processName,
            Ordinal ordinal = MAIN);
    void addProcess(IFrameProcess *processToAdd,
            Privilege privilegeLevel,
            Ordinal ordinal);
    void addProcess(IFrameProcess *processToAdd,
            Ordinal ordinal);


    // Metrics
    /** @brief Metrics manager object. */
    FrameMetrics metrics;


    // Control flow
    /** @brief Begins frame processes. */
    void run();

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
    bool shouldRunLoop();

    // Internal Types
    struct ProcessInstance {
        IFrameProcess *frameProcess;
        Privilege privilegeLevel = NONE;
        int processID = -1;
        std::string processName = "anonymous";
    };

    struct ProcessIterator {
     public:
        std::list<ProcessInstance*> early;
        std::list<ProcessInstance*> main;
        std::list<ProcessInstance*> late;

        std::list<ProcessInstance*>::iterator currentPointer;
        std::list<ProcessInstance*>* currentList;

        FrameController::ProcessInstance* back();
        std::list<ProcessInstance*>::iterator begin();
        std::list<ProcessInstance*>::iterator end();
        std::list<ProcessInstance*>::iterator next();
        void rectify();
        int size();
    };

    class ProcessManager {
     public:
        friend class FrameController;

    #ifndef TEST_BUILD

     private:
    #endif
        explicit ProcessManager(
            std::shared_ptr<ITimerSource> timerSource = nullptr);
        ProcessIterator processes;
        std::shared_ptr<ITimerSource> timerSource;

        void addProcess(ProcessInstance *newProcess,
            Ordinal ordinal = Ordinal::MAIN);
        void addEarlyProcess(ProcessInstance *newProcess);
        void addMainProcess(ProcessInstance *newProcess);
        void addLateProcess(ProcessInstance *newProcess);
        void removeProcess(ProcessInstance *processToRemove);
        bool hasProcesses();

        void runStart();
        void runLoop();
        void runStop();
        void runMethod(std::function<void(IFrameProcess*)> method);

        FCState currentState = RUNNING;
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
