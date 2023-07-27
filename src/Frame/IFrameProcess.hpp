#ifndef SRC_FRAME_IFRAMEPROCESS_HPP_
#define SRC_FRAME_IFRAMEPROCESS_HPP_

#include <string>

using std::string;

/**
 * @brief Struct containing metadata for frame process.
 */
struct ProcessMetadata {
 public:
    /** @brief Human-readable process name. */
    string name = "Unnamed";

    /** @brief Unique process identifier.
     *  Value of -1 is untracked. */
    int id = -1;
};

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

    /** @brief Metadata for process. */
    ProcessMetadata metadata;

 protected:
    State currentState = READY;
};

#endif  // SRC_FRAME_IFRAMEPROCESS_HPP_
