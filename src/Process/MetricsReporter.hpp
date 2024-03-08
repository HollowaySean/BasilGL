#ifndef SRC_PROCESS_METRICSREPORTER_HPP_
#define SRC_PROCESS_METRICSREPORTER_HPP_

#include <fmt/core.h>

#include <memory>
#include <string>

#include <Basil/Logging.hpp>

#include "IProcess.hpp"
#include "MetricsObserver.hpp"
#include "ProcessController.hpp"

namespace basil {

/** @brief Utility FrameProcess that prints frame rate to
 *  the command line.
*/
class MetricsReporter : public IProcess {
 public:
    /** @brief Create MetricsReporter object. */
    explicit MetricsReporter(
        std::optional<std::shared_ptr<ProcessController>> controller = std::nullopt);

    /** @brief Set pointer to ProcessController to monitor. */
    void setController(std::shared_ptr<ProcessController> controller);

    /** @brief Main loop function for ProcessController. */
    void onLoop() override;

    /** @return Number of frames per print out. */
    unsigned int getRegularity() { return regularity; }

    /** @brief Set number of frames per print out. */
    void setRegularity(unsigned int regularity) {
        this->regularity = regularity;
    }

#ifndef TEST_BUILD

 private:
#endif
    std::shared_ptr<MetricsObserver> metrics;

    Logger& logger = Logger::get();
    LogLevel logLevel = LogLevel::INFO;

    unsigned int regularity = 30;
};

}  // namespace basil

#endif  // SRC_PROCESS_METRICSREPORTER_HPP_
