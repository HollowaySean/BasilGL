#ifndef SRC_FRAME_METRICSREPORTER_HPP_
#define SRC_FRAME_METRICSREPORTER_HPP_

#include "IFrameProcess.hpp"
#include "FrameMetrics.hpp"

/** @brief Utility FrameProcess that prints frame rate to
 *  the command line.
*/
class MetricsReporter : public IFrameProcess {
 public:
    explicit MetricsReporter(
        FrameMetrics *metricsObserver,
        int regularity = 1);

    void onLoop() override;

    /** @brief Number of frames per print out. */
    int regularity;

 private:
    FrameMetrics *metrics;
};

#endif  // SRC_FRAME_METRICSREPORTER_HPP_
