#ifndef SRC_FRAME_DEFAULTTIMERSOURCE_HPP_
#define SRC_FRAME_DEFAULTTIMERSOURCE_HPP_

#include <memory>
#include <thread>

#include "ITimerSource.hpp"

namespace basil {

using std::chrono::steady_clock;

/**
 * @brief Default implementation of ITimerSource.
 * Uses chrono and thread libraries to measure time and sleep the thread.
 */
// TODO(sholloway): Rename this class
// TODO(sholloway): Add documentation
class DefaultTimerSource: public ITimerSource {
 public:
    std::unique_ptr<ITimerSource> clone() const override;
    void frameStart() override;
    void frameDone() override;
    void waitForFrameTime() override;
    void processStart(int processID) override;
    void processDone(int processID) override;
    double readTimer() const override;

#ifndef TEST_BUILD

 private:
#endif
    constexpr static double SECONDS_PER_NANO = 1e9;

    using duration = std::chrono::duration<double>;
    using time_point = steady_clock::time_point;
    static duration secondsToDuration(double timeInSeconds);
    static double timePointToSeconds(time_point timePoint);
};

}  // namespace basil

#endif  // SRC_FRAME_DEFAULTTIMERSOURCE_HPP_
