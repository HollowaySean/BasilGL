#ifndef SRC_FRAME_ITIMERSOURCE_HPP_
#define SRC_FRAME_ITIMERSOURCE_HPP_

/**
 * @brief Interface consumable by FrameController
 * Provides time stamps and wait functionality.
 * Useful for test stubbing and non-realtime controls.
 */
class ITimerSource {
 public:
    /** @param waitTimeInMS Number of milliseconds that must
     *  elapse before wait finishes. */
    virtual void setMinimumWaitTime(float waitTimeInMS) = 0;

    /** @brief  Begin timer and return timestamp
     *  @return Timestamp as an integer */
    virtual float startTimer() = 0;

    /** @brief  Return timestamp without stopping */
    virtual float readTimer() = 0;

    /** @brief  Stop timer and return timestamp
     *  @return Timestamp as an integer */
    virtual float stopTimer() = 0;

    /** @brief  Wait for time equal to minimumWaitTime - elapsedTime
     *  @return Time waited as an integer */
    virtual float waitForTime() = 0;
};

#endif  // SRC_FRAME_ITIMERSOURCE_HPP_
