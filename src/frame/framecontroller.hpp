#ifndef SRC_FRAME_FRAMECONTROLLER_HPP_
#define SRC_FRAME_FRAMECONTROLLER_HPP_

#include <functional>
#include <set>
#include <thread>

// Hoisting
class FrameController;

/**
 * @brief Interface consumable by FrameController
 * Contains 'mainLoop' function which is run within frame.
 */
class Runnable {
 public:
    /** @brief Function to run before first frame. */
    virtual void onStart() {}
    /** @brief Function to run repeatedly within successive frames. */
    virtual void mainLoop() = 0;
    /** @brief Function to run after last frame. */
    virtual void onStop() {}
    /** @brief Associate Controller for feedback. */
    void setController(FrameController *controller) {
        frameController = controller;
    }

 protected:
    FrameController *frameController;
};

/**
 * @brief Interface consumable by FrameController
 * Provides time stamps and wait functionality.
 * Useful for test stubbing and non-realtime controls.
 */
class TimerSource {
 public:
    /** @param waitTimeInMS Number of milliseconds that must
     *  elapse before wait finishes. */
    virtual void setMinimumWaitTime(int waitTimeInMS) = 0;
    /** @brief  Begin timer and return timestamp
     *  @return Timestamp as an integer */
    virtual int startTimer() = 0;
    /** @brief  Stop timer and return timestamp
     *  @return Timestamp as an integer */
    virtual int stopTimer() = 0;
    /** @brief  Wait for time equal to minimumWaitTime - elapsedTime
     *  @return Time waited as an integer */
    virtual int waitForTime() = 0;
};

/**
 * @brief Default implementation of TimerSource.
 * Uses chrono and thread libraries to measure time and sleep the thread.
 */
class DefaultTimerSource: public TimerSource {
 public:
    DefaultTimerSource() = default;
    void setMinimumWaitTime(int waitTimeInMS) override;
    int startTimer() override;
    int stopTimer() override;
    int waitForTime() override;

 private:
    std::chrono::steady_clock::duration minimumWaitTime
        = std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::duration elapsedTime
        = std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::time_point timerStart
        = std::chrono::steady_clock::time_point::min();
    std::chrono::steady_clock::time_point timerStop
        = std::chrono::steady_clock::time_point::min();
};

/**
 * @brief Utility to run main loop of Runnable class, monitor the
 * running time, and limit to a given frame rate.
 */
class FrameController {
 public:
    /** @brief Creates new FrameController with optional argument of Runnable,
     *  which otherwise defaults to null. */
    explicit FrameController(
        Runnable *runnable = nullptr,
        TimerSource *timerSource = nullptr);

    /** @brief Indicates current status of FrameController. */
    enum FrameControllerState {
        STOPPING,
        STOPPED,
        RUNNING,
    };


    // Setters
    /** @param newRunnable Pointer to Runnable to add to list. */
    void addRunnable(Runnable *newRunnable);
    /** @param runnableToRemove Pointer to Runnable to try-remove from list. */
    void removeRunnable(Runnable *runnableToRemove);

    /** @param framesPerSecond Maximum frame rate.
     *  Uncapped frame rate when set to 0. */
    void setFrameCap(int framesPerSecond);

    /** @param newTimerSource Pointer to instance of TimerSource to use. */
    void setTimerSource(TimerSource *newTimerSource) {
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

 private:
    // Timing variables
    TimerSource *timerSource;
    int frameCap;

    // Collection of runnable classes
    std::set<Runnable*> runnables;

    // State management
    FrameControllerState currentState;
    bool shouldRunLoop();
    bool shouldStartLoop();

    // Controls
    void runLoop();
};

#endif  // SRC_FRAME_FRAMECONTROLLER_HPP_
