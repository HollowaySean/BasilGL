#ifndef SRC_FRAME_METRICSREPORTER_HPP_
#define SRC_FRAME_METRICSREPORTER_HPP_

#include "IFrameProcess.hpp"
#include "FrameMetrics.hpp"

// TODO(sholloway): Documentation, cleanup
class MetricsReporter : public IFrameProcess {
 public:
    explicit MetricsReporter(
        FrameMetrics *metricsObserver,
        int regularity = 1);

    void onLoop() override;

    int regularity;

 private:
    FrameMetrics *metrics;
};

#endif  // SRC_FRAME_METRICSREPORTER_HPP_
