#ifndef SRC_FRAME_IRUNNABLE_HPP_
#define SRC_FRAME_IRUNNABLE_HPP_

#include <string>

/**
 * @brief Interface consumable by FrameController
 * Contains 'mainLoop' function which is run within frame.
 */
class IRunnable {
 public:
    /** @brief Enum indicating possible states of Runnable
     *  that would be visible to the owning FrameController.
     *  Enum ordered by priority/severity. */
    enum State {
        READY, SKIP, REQUEST_STOP, REQUEST_KILL
    };

    /** @return Current state of Runnable. */
    virtual State getCurrentState() { return currentState; }

    /** @return Plaintext name for metrics reporting. */
    virtual std::string getName() { return "Unnamed"; }

    /** @brief Function to run before first frame. */
    virtual void onStart() {}

    /** @brief Function to run repeatedly within successive frames. */
    virtual void mainLoop() = 0;

    /** @brief Function to run after last frame. */
    virtual void onStop() {}

 protected:
    State currentState = READY;
};

#endif  // SRC_FRAME_IRUNNABLE_HPP_
