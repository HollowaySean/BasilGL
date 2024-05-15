#ifndef SRC_DATA_SYSTEMTIMEMODEL_HPP_
#define SRC_DATA_SYSTEMTIMEMODEL_HPP_

#include <Basil/Process.hpp>

namespace basil {

struct SystemTimeModel {
    FrameClock::duration timeSinceStart = FrameClock::duration::zero();
    FrameClock::duration timeSinceFrame = FrameClock::duration::zero();
    int frameNumber = 0;

    double frameRate() {
        if (frameNumber == 0) return 0;

        return FrameTimer::periodToFrequency(timeSinceStart / frameNumber);
    }
};

}  // namespace basil

#endif  // SRC_DATA_SYSTEMTIMEMODEL_HPP_
