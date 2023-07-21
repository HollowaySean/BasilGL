#ifndef SRC_FRAME_FRAMECONTROLLER_HPP_
#define SRC_FRAME_FRAMECONTROLLER_HPP_

#include <chrono>
#include <functional>

class FrameController {
 public:
    // Constructors
    FrameController();
    explicit FrameController(std::function<void()> loopFunction);

    // Internal Types
    enum FrameControllerState {
        STOPPING,
        STOPPED,
        RUNNING,
    };

    // Setters
    void setLoopFunction(std::function<void()> newLoopFunction);
    void setFrameCap(int framesPerSecond);

    // Getters
    FrameControllerState getCurrentState() { return currentState; }
    int getFrameCap() { return frameCap; }

    // Controls
    void start();
    void stop();

 private:
    // Timing variables
    std::chrono::steady_clock::time_point loopStart;
    std::chrono::steady_clock::time_point loopEnd;
    std::chrono::steady_clock::duration loopTime;
    std::chrono::steady_clock::duration frameTime;
    int frameCap;

    // State management
    std::function<void()> loopFunction;
    FrameControllerState currentState;
    bool shouldRunLoop();
    bool shouldStartLoop();

    // Controls
    void runLoop();
    void waitForFrameEnd();
};


#endif  // SRC_FRAME_FRAMECONTROLLER_HPP_
