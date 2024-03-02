#ifndef SRC_FRAME_ITIMERSOURCE_HPP_
#define SRC_FRAME_ITIMERSOURCE_HPP_

#include <map>
#include <memory>

namespace basil {

/**
 * @brief Record of frame and process times.
*/
struct TimerRecord {
 public:
    explicit TimerRecord(int newFrameID = 0) {
        frameID = newFrameID;
    }

    /** @brief Frame ID number. */
    int frameID;

    /** @brief Time at frame start. */
    double frameStart = 0;

    /** @brief Time at all processes done. */
    double frameDone = 0;

    /** @brief Time at frame end. */
    double frameEnd = 0;

    /** @brief Time at process start, indexed by processID. */
    std::map<int, double> processStart;

    /** @brief Time at process done, indexed by processID. */
    std::map<int, double> processDone;
};

/**
 * @brief Interface consumable by FrameController
 * Provides time stamps and wait functionality.
 * Useful for test stubbing and non-realtime controls.
 */
class ITimerSource {
 public:
    ITimerSource(): currentRecord() {
        currentRecord = TimerRecord(0);
    }

    /** @brief Copy constructor for creating child pointers */
    // TODO(sholloway): Make it so this isn't necessary
    virtual std::unique_ptr<ITimerSource> clone() const = 0;

    /** @param waitTimeInSeconds Time in seconds that must
     *  elapse before wait finishes. */
    virtual void setMinimumFrameTime(double newWaitTimeInSeconds) {
        waitTimeInSeconds = newWaitTimeInSeconds;
    }

    /** @brief Starts timer at beginning of frame. */
    virtual void frameStart() = 0;

    /** @brief Stops timer at the end of frame.
     *  Indicates that all processes are complete. */
    virtual void frameDone() = 0;

    /** @brief Waits until minimum frame time has elapsed. */
    virtual void waitForFrameTime() = 0;

    /** @brief Starts timer at beginning of process. */
    virtual void processStart(int processID) = 0;

    /** @brief Starts timer at beginning of process. */
    virtual void processDone(int processID) = 0;

    /** @brief  Return timestamp without recording. */
    virtual double readTimer() const = 0;

    /** @return Record of timestamps for frame. */
    virtual TimerRecord getRecord() const {
        return currentRecord;
    }

#ifndef TEST_BUILD

 protected:
#endif
    int frameID = 0;
    double waitTimeInSeconds = 0;
    TimerRecord currentRecord;
};

}  // namespace basil

#endif  // SRC_FRAME_ITIMERSOURCE_HPP_
