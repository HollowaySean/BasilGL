#pragma once

#include <Basil/App.hpp>

namespace basil {

/** @brief Widget which kills ProcessController after a set number
 *  of frames or after a set amount of time. */
class StopAfterTime : public IBasilWidget,
                      public IBuildable<StopAfterTime> {
 public:
    /** @brief Initializes StopAfterTime widget */
    StopAfterTime();

    /** @brief Set number of frames to run before stopping */
    void setFramesUntilStop(unsigned int numberOfFrames);

    /** @brief Sets amount of time to run before stopping*/
    void setTimeUntilStop(float timeUntilStopInSeconds);

    /** @brief IProcess override, initializes start time/frame */
    void onStart() override;

    /** @brief IProcess override, checks current time/frame */
    void onLoop() override;

    /** @brief Builder pattern for widget */
    class Builder : public IBuilder<StopAfterTime> {
     public:
        /** @brief Build with time limit */
        Builder& setTimeUntilStop(float timeUntilStopInSeconds);

        /** @brief Build with frame limit */
        Builder& setFramesUntilStop(unsigned int numberOfFrames);
    };

#ifndef TEST_BUILD

 private:
#endif
    // Large amount of time but without risk of overflow
    FrameClock::duration timeToRun = std::chrono::years(1);
    FrameClock::time_point stopTime = FrameClock::time_point::max();

    unsigned int framesToRun = -1;
    unsigned int stopFrame = -1;
    unsigned int currentFrame = 0;
};

}   // namespace basil
