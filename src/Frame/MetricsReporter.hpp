#ifndef SRC_FRAME_METRICSREPORTER_HPP_
#define SRC_FRAME_METRICSREPORTER_HPP_

#include <fmt/core.h>

#include <map>
#include <string>

#include "IFrameProcess.hpp"
#include "FrameMetrics.hpp"
#include <Basil/Logging.hpp>

namespace basil {

/** @brief Utility FrameProcess that prints frame rate to
 *  the command line.
*/
class MetricsReporter : public IFrameProcess {
 public:
    /**
     * @param metricsObserver   FrameMetrics object to report on.
     * @param regularity        Number of frames per report.
     * @param logLevel          Severity level to log report at.
    */
    explicit MetricsReporter(
        FrameMetrics *metricsObserver,
        int regularity = 1,
        LogLevel logLevel = LogLevel::INFO);

    void onLoop() override;

    /** @brief Number of frames per print out. */
    int regularity;

#ifndef TEST_BUILD

 private:
#endif
    FrameMetrics *metrics;
    std::map<int, std::string> *processNames;

    Logger& logger = Logger::get();
    LogLevel logLevel;

    fmt::format_string<int> FRAME_FORMAT
        = "Frame #: {}";
    fmt::format_string<double> FRAME_RATE_FORMAT
        = "Frame rate: {:.2f}";
    fmt::format_string<double> MAX_FRAME_RATE_FORMAT
        = "Max frame rate: {:.2f}";
    fmt::format_string<const char*, double> PROCESS_TIME_FORMAT
        = "Process \'{}\': {:.3f}ms";
};

}  // namespace basil

#endif  // SRC_FRAME_METRICSREPORTER_HPP_
