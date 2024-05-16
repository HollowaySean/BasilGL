#ifndef SRC_WIDGET_STOPAFTERTIME_HPP_
#define SRC_WIDGET_STOPAFTERTIME_HPP_

#include <Basil/App.hpp>

namespace basil {

// TODO(sholloway): Documentation
// TODO(sholloway): Test
class StopAfterTime : public IBasilWidget,
                      public IBuildable<StopAfterTime> {
 public:
    StopAfterTime() : IBasilWidget({
        "StopAfterTime",
        ProcessOrdinal::LATE,
        ProcessPrivilege::HIGH,
        WidgetPubSubPrefs::NONE
    }) {}

    void setFramesUntilStop(unsigned int numberOfFrames) {
        framesToRun = numberOfFrames;
    }

    void setTimeUntilStop(float timeUntilStopInSeconds) {
        timeToRun = std::chrono::milliseconds(
            static_cast<int>(timeUntilStopInSeconds * 1'000.f));
    }

    void onStart() override {
        auto startTime = FrameClock::now();
        stopTime = startTime + timeToRun;

        stopFrame = currentFrame + framesToRun;
    }

    void onLoop() override {
        auto currentTime = FrameClock::now();

        if (currentFrame >= stopFrame || currentTime >= stopTime) {
            setCurrentState(ProcessState::REQUEST_STOP);
        }

        currentFrame++;
    }

    class Builder : public IBuilder<StopAfterTime> {
     public:
        Builder& setTimeUntilStop(float timeUntilStopInSeconds) {
            this->impl->setTimeUntilStop(timeUntilStopInSeconds);
            return (*this);
        }

        Builder& setFramesUntilStop(unsigned int numberOfFrames) {
            this->impl->setFramesUntilStop(numberOfFrames);
            return (*this);
        }
    };

 private:
    // Large amount of time but without risk of overflow
    FrameClock::duration timeToRun = std::chrono::years(1);
    FrameClock::time_point stopTime = FrameClock::time_point::max();

    unsigned int framesToRun = -1;
    unsigned int stopFrame = -1;
    unsigned int currentFrame = 0;
};

}  // namespace basil

#endif  // SRC_WIDGET_STOPAFTERTIME_HPP_
