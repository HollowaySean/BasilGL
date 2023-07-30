#ifndef SRC_FRAME_FRAMECONTROLLER_HPP_
#define SRC_FRAME_FRAMECONTROLLER_HPP_

#include <functional>
#include <list>
#include <string>

#include "FrameMetrics.hpp"
#include "IFrameProcess.hpp"
#include "ITimerSource.hpp"

/**
 * @brief Utility to run main loop of IFrameProcess class, monitor the
 * running time, and limit to a given frame rate.
 */
class FrameController {
 public:
    /** @brief Creates new FrameController with optional argument of IFrameProcess,
     *  which otherwise defaults to null. */
    explicit FrameController(
        IFrameProcess *runnable = nullptr,
        ITimerSource *timerSource = nullptr);

    /** @brief Indicates current status of FrameController. */
    enum FrameControllerState {
        RUNNING, STOPPING, STOPPED, KILLED
    };

    /** @brief Enum for positional options to add new runnable. */
    enum Order {
        FIRST, LAST, BEFORE, AFTER
    };

    // /** @brief Adds runnable to run in the middle of the frame.
    //  *  @param newRunnable   Pointer to IFrameProcess to add to list.
    //  *  @param orderToInsert Optional order enum indicating position to add.
    //  *  @param relativeTo    Reference point for BEFORE or AFTER ordinal. */
    // void addRunnable(IFrameProcess *newRunnable,
    //     Order orderToInsert = Order::LAST,
    //     IFrameProcess *relativeTo = nullptr);

    // /** @brief Adds runnable to run at the beginning of the frame.
    //  *  @param newRunnable   Pointer to IFrameProcess to add to list.
    //  *  @param orderToInsert Optional order enum indicating position to add.
    //  *  @param relativeTo    Reference point for BEFORE or AFTER ordinal. */
    // void addEarlyRunnable(IFrameProcess *newRunnable,
    //     Order orderToInsert = Order::LAST,
    //     IFrameProcess *relativeTo = nullptr);

    // /** @brief Adds runnable to run at the end of the frame.
    //  *  @param newRunnable   Pointer to IFrameProcess to add to list.
    //  *  @param orderToInsert Optional order enum indicating position to add.
    //  *  @param relativeTo    Reference point for BEFORE or AFTER ordinal. */
    // void addLateRunnable(IFrameProcess *newRunnable,
    //     Order orderToInsert = Order::LAST,
    //     IFrameProcess *relativeTo = nullptr);

    void addRunnable(
        IFrameProcess *newRunnable,
        Order orderToInsert = Order::LAST);

    /** @param runnableToRemove
     *      Pointer to IFrameProcess to try-remove from list. */
    void removeRunnable(IFrameProcess *runnableToRemove);

    /** @param framesPerSecond Maximum frame rate.
     *  Uncapped frame rate when set to 0. */
    void setFrameCap(int framesPerSecond);

    /** @param newTimerSource Pointer to instance of ITimerSource to use. */
    void setTimerSource(ITimerSource *newTimerSource) {
        timerSource = newTimerSource;
    }


    // Getters
    /** @return FrameController::FrameControllerState enum
     *  indicating current status. */
    FrameControllerState getCurrentState() { return currentState; }

    /** @return Maximum frame rate cap. Uncapped frame rate if set to 0. */
    int getFrameCap() { return frameCap; }


    // Controls
    /** @brief Begin running main loop of Runnable. */
    void start();

    /** @brief Request stop of main loop. */
    void stop();

    /** @brief Demand stop of main loop. Interrupts runnables. */
    void kill();

    // Metrics
    /** @brief Frame time/rate metrics recording utility. */
    FrameMetrics frameMetrics;

 private:
    // Timing variables
    ITimerSource *timerSource;
    int frameCap;

    // State management
    FrameControllerState currentState;
    bool shouldRunLoop();
    bool shouldStartLoop();
    bool shouldKillLoop();

    // Controls
    void runLoop();

    // RUNNABLE MANAGER MIGRATION

    // Collections
    std::list<IFrameProcess*> earlyRunnables;
    std::list<IFrameProcess*> mainRunnables;
    std::list<IFrameProcess*> lateRunnables;

    // FrameController-friendly functions
    void safelyRunCollection(std::function<void()> method);
    void checkRunnableState(IFrameProcess *runnable);
    void runStart();
    void runMain();
    void runStop();
};

#endif  // SRC_FRAME_FRAMECONTROLLER_HPP_
