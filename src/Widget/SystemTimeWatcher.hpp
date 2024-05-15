#ifndef SRC_WIDGET_SYSTEMTIMEWATCHER_HPP_
#define SRC_WIDGET_SYSTEMTIMEWATCHER_HPP_

#include <Basil/App.hpp>
#include <Basil/Data.hpp>

namespace basil {

// TODO(sholloway): Create interface for watcher/model pattern?
class SystemTimeWatcher : public IBasilWidget {
 public:
    SystemTimeWatcher();

    void onStart() override;

    void onLoop() override;

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
