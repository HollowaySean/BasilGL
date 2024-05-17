#pragma once

#include <Basil/Packages/Chrono.hpp>

namespace basil {

/** @brief Struct to hold data required for frame time reporting */
struct SystemTimeModel {
    /** @brief Time since monitoring process has started */
    FrameClock::duration timeSinceStart = FrameClock::duration::zero();

    /** @brief Time since previous frame was reported */
    FrameClock::duration timeSinceFrame = FrameClock::duration::zero();

    /** @brief # of frames elapsed */
    int frameNumber = 0;

    /** @returns Average frame rate across all collection */
    double frameRate() {
        if (frameNumber == 0) return 0;

        return FrameTimer::periodToFrequency(timeSinceStart / frameNumber);
    }
};

}   // namespace basil
