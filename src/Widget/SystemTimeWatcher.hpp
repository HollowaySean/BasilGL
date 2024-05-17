#pragma once

#include <Basil/App.hpp>
#include <Basil/Data.hpp>

namespace basil {

/** @brief IBasilWidget that records and publishes frame and process time */
class SystemTimeWatcher : public IBasilWidget,
                          public IBuildable<SystemTimeWatcher> {
 public:
    /** @brief Initialize SystemTimeWatcher */
    SystemTimeWatcher();

    /** @brief Initialize SystemTimeModel values */
    void onStart() override;

    /** @brief Update model based on FrameClock */
    void onLoop() override;

    /** @returns Reference to SystemTimeModel */
    SystemTimeModel& getModel() {
        return model;
    }

    /** @brief Builder pattern for SystemTimeWatcher */
    class Builder : public IBuilder<SystemTimeWatcher> {};

 private:
    SystemTimeModel model = SystemTimeModel();

    FrameClock::time_point startTime;
    FrameClock::time_point lastFrameTime;
};

}   // namespace basil
