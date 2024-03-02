#ifndef SRC_FRAME_IFRAMEPROCESS_HPP_
#define SRC_FRAME_IFRAMEPROCESS_HPP_

#include <string>

namespace basil {

/**
 * @brief Interface consumable by FrameController
 * Contains 'onLoop' function which is run within frame.
 */
class IFrameProcess {
 public:
    /** @brief Enum indicating possible states of process
     *  that would be visible to the owning FrameController.
     *  Enum ordered by priority/severity. */
    enum State {
        READY,
        SKIP_PROCESS,
        REMOVE_PROCESS,
        REQUEST_STOP,
        REQUEST_KILL
    };

    /** @return Current state of process. */
    virtual State getCurrentState() { return currentState; }

    /** @brief Function to run before first frame. */
    virtual void onStart() {}

    /** @brief Function to run repeatedly within successive frames. */
    virtual void onLoop() = 0;

    /** @brief Function to run after last frame. */
    virtual void onStop() {}

#ifndef TEST_BUILD

 protected:
#endif
    State currentState = READY;
};

}  // namespace basil

#endif  // SRC_FRAME_IFRAMEPROCESS_HPP_
