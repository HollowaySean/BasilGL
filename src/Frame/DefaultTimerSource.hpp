#ifndef SRC_FRAME_DEFAULTTIMERSOURCE_HPP_
#define SRC_FRAME_DEFAULTTIMERSOURCE_HPP_

#include <thread>

#include "ITimerSource.hpp"

using std::chrono::steady_clock;

/**
 * @brief Default implementation of ITimerSource.
 * Uses chrono and thread libraries to measure time and sleep the thread.
 */
class DefaultTimerSource: public ITimerSource {
 public:
    void setMinimumWaitTime(double waitTimeInSeconds) override;
    double startTimer() override;
    double readTimer() override;
    double stopTimer() override;
    double waitForTime() override;

 private:
    using duration = std::chrono::duration<double>;
    using time_point = steady_clock::time_point;
    duration secondsToDuration(double timeInSeconds);
    double timePointToMilliseconds(time_point timePoint);
    double durationToMilliseconds(duration duration);
    duration minimumWaitTime
        = steady_clock::duration::zero();
    duration elapsedTime() { return timerStop - timerStart; }
    time_point timerStart
        = steady_clock::time_point::min();
    time_point timerStop
        = steady_clock::time_point::min();
};

#endif  // SRC_FRAME_DEFAULTTIMERSOURCE_HPP_