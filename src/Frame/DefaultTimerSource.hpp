#ifndef SRC_FRAME_DEFAULTTIMERSOURCE_HPP_
#define SRC_FRAME_DEFAULTTIMERSOURCE_HPP_

#include <thread>

#include "ITimerSource.hpp"

/**
 * @brief Default implementation of ITimerSource.
 * Uses chrono and thread libraries to measure time and sleep the thread.
 */
class DefaultTimerSource: public ITimerSource {
 public:
    DefaultTimerSource() = default;
    void setMinimumWaitTime(float waitTimeInMS) override;
    float startTimer() override;
    float readTimer() override;
    float stopTimer() override;
    float waitForTime() override;

 private:
    std::chrono::duration<float> minimumWaitTime
        = std::chrono::steady_clock::duration::zero();
    std::chrono::duration<float>  elapsedTime
        = std::chrono::steady_clock::duration::zero();
    std::chrono::steady_clock::time_point timerStart
        = std::chrono::steady_clock::time_point::min();
    std::chrono::steady_clock::time_point timerStop
        = std::chrono::steady_clock::time_point::min();
};

#endif  // SRC_FRAME_DEFAULTTIMERSOURCE_HPP_
