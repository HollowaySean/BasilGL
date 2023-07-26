#ifndef SRC_FRAME_FRAMECONTROLLER_HPP_
#define SRC_FRAME_FRAMECONTROLLER_HPP_

#include <functional>
#include <list>
#include <string>

#include "FrameMetrics.hpp"
#include "IRunnable.hpp"
#include "ITimerSource.hpp"

/**
 * @brief Utility to run main loop of IRunnable class, monitor the
 * running time, and limit to a given frame rate.
 */
class FrameController {
 public:
    /** @brief Creates new FrameController with optional argument of IRunnable,
     *  which otherwise defaults to null. */
    explicit FrameController(
        IRunnable *runnable = nullptr,
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
    //  *  @param newRunnable   Pointer to IRunnable to add to list.
    //  *  @param orderToInsert Optional order enum indicating position to add.
    //  *  @param relativeTo    Reference point for BEFORE or AFTER ordinal. */
    // void addRunnable(IRunnable *newRunnable,
    //     Order orderToInsert = Order::LAST,
    //     IRunnable *relativeTo = nullptr);

    // /** @brief Adds runnable to run at the beginning of the frame.
    //  *  @param newRunnable   Pointer to IRunnable to add to list.
    //  *  @param orderToInsert Optional order enum indicating position to add.
    //  *  @param relativeTo    Reference point for BEFORE or AFTER ordinal. */
    // void addEarlyRunnable(IRunnable *newRunnable,
    //     Order orderToInsert = Order::LAST,
    //     IRunnable *relativeTo = nullptr);

    // /** @brief Adds runnable to run at the end of the frame.
    //  *  @param newRunnable   Pointer to IRunnable to add to list.
    //  *  @param orderToInsert Optional order enum indicating position to add.
    //  *  @param relativeTo    Reference point for BEFORE or AFTER ordinal. */
    // void addLateRunnable(IRunnable *newRunnable,
    //     Order orderToInsert = Order::LAST,
    //     IRunnable *relativeTo = nullptr);

    void addRunnable(IRunnable *newRunnable, Order orderToInsert = Order::LAST);

    /** @param runnableToRemove Pointer to IRunnable to try-remove from list. */
    void removeRunnable(IRunnable *runnableToRemove);

    /** @param framesPerSecond Maximum frame rate.
     *  Uncapped frame rate when set to 0. */
    void setFrameCap(int framesPerSecond);

    /** @param newTimerSource Pointer to instance of ITimerSource to use. */
    void setTimerSource(ITimerSource *newTimerSource) {
        timerSource = newTimerSource;
    }


    // Getters
    /** @returns FrameController::FrameControllerState enum
     *  indicating current status. */
    FrameControllerState getCurrentState() { return currentState; }

    /** @returns Maximum frame rate cap. Uncapped frame rate if set to 0. */
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
    std::list<IRunnable*> earlyRunnables;
    std::list<IRunnable*> mainRunnables;
    std::list<IRunnable*> lateRunnables;

    // FrameController-friendly functions
    void safelyRunCollection(std::function<void()> method);
    void checkRunnableState(IRunnable *runnable);
    void runStart();
    void runMain();
    void runStop();
};

#endif  // SRC_FRAME_FRAMECONTROLLER_HPP_
