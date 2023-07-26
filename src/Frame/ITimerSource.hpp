#ifndef SRC_FRAME_ITIMERSOURCE_HPP_
#define SRC_FRAME_ITIMERSOURCE_HPP_

/**
 * @brief Interface consumable by FrameController
 * Provides time stamps and wait functionality.
 * Useful for test stubbing and non-realtime controls.
 */
class ITimerSource {
 public:
    /** @param waitTimeInSeconds Time in seconds that must
     *  elapse before wait finishes. */
    virtual void setMinimumWaitTime(double waitTimeInSeconds) = 0;

    /** @brief  Begin timer and return timestamp
     *  @return Timestamp as an integer */
    virtual double startTimer() = 0;

    /** @brief  Return timestamp without stopping */
    virtual double readTimer() = 0;

    /** @brief  Stop timer and return timestamp
     *  @return Timestamp as an integer */
    virtual double stopTimer() = 0;

    /** @brief  Wait for time equal to minimumWaitTime - elapsedTime
     *  @return Time waited as an integer */
    virtual double waitForTime() = 0;
};

#endif  // SRC_FRAME_ITIMERSOURCE_HPP_
