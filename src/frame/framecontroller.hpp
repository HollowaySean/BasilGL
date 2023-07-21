#ifndef SRC_FRAME_FRAMECONTROLLER_HPP_
#define SRC_FRAME_FRAMECONTROLLER_HPP_

#include <chrono>
#include <functional>

/**
 * @brief Interface consumable by FrameController
 * Contains 'mainLoop' function which is run within frame
 * 
 */
class Runnable {
 public:
    virtual void mainLoop() = 0;
};

/**
 * @brief Utility to run main loop of Runnable class, monitor the
 * running time, and limit to a given frame rate.
 */
class FrameController {
 public:
    /** @brief Creates new FrameController with optional argument of Runnable,
     *  which otherwise defaults to null. */
    explicit FrameController(Runnable *runnable);

    /** @brief Indicates current status of FrameController. */
    enum FrameControllerState {
        STOPPING,
        STOPPED,
        RUNNING,
    };


    // Setters
    /** @param newRunnable Pointer to instance of Runnable to use. */
    void setRunnable(Runnable *newRunnable) { runnable = newRunnable; }

    /** @param framesPerSecond Maximum frame rate.
     *  Uncapped frame rate when set to 0. */
    void setFrameCap(int framesPerSecond);


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
    std::chrono::steady_clock::time_point loopStart;
    std::chrono::steady_clock::time_point loopEnd;
    std::chrono::steady_clock::duration loopTime;
    std::chrono::steady_clock::duration frameTime;
    int frameCap;

    // State management
    Runnable *runnable = nullptr;
    FrameControllerState currentState;
    bool shouldRunLoop();
    bool shouldStartLoop();

    // Controls
    void runLoop();
    void waitForFrameEnd();
};

#endif  // SRC_FRAME_FRAMECONTROLLER_HPP_
