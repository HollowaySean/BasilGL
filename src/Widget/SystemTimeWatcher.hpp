#ifndef SRC_WIDGET_SYSTEMTIMEWATCHER_HPP_
#define SRC_WIDGET_SYSTEMTIMEWATCHER_HPP_

#include <Basil/App.hpp>
#include <Basil/Data.hpp>

namespace basil {

/** @brief IBasilWidget that records and publishes frame and process time */
class SystemTimeWatcher : public IBasilWidget {
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

 private:
    SystemTimeModel model = SystemTimeModel();

    FrameClock::time_point startTime;
    FrameClock::time_point lastFrameTime;
};

}  // namespace basil

#endif  // SRC_WIDGET_SYSTEMTIMEWATCHER_HPP_
