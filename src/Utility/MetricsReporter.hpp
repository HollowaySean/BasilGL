#ifndef SRC_UTILITY_METRICSREPORTER_HPP_
#define SRC_UTILITY_METRICSREPORTER_HPP_

#include <fmt/core.h>

#include <memory>
#include <string>

#include <Basil/Logging.hpp>
#include <Basil/Process.hpp>

namespace basil {

/** @brief Utility FrameProcess that prints frame rate to
 *  the command line.
*/
class MetricsReporter : public IProcess,
                        public IBuildable<MetricsReporter> {
 public:
    /** @brief Create MetricsReporter object. */
    MetricsReporter();

    /** @brief Main loop function for ProcessController. */
    void onLoop() override;

    /** @return Number of frames per print out. */
    unsigned int getRegularity() { return regularity; }

    /** @brief Set number of frames per print out. */
    void setRegularity(unsigned int regularity) {
        this->regularity = regularity;
    }

    /** @brief Set severity level for logging. */
    void setLogLevel(LogLevel level) {
        this->logLevel = level;
    }

    /** @brief Builder pattern for MetricsReporter. */
    class Builder : public IBuilder<MetricsReporter> {
     public:
        /** @brief Set number of frames per print out. */
        Builder& withRegularity(unsigned int regularity);

        /** @brief Set severity level for logging*/
        Builder& withLogLevel(LogLevel level);
    };

#ifndef TEST_BUILD

 private:
#endif
    Logger& logger = Logger::get();
    LogLevel logLevel = LogLevel::INFO;

    unsigned int regularity = 30;
};

}  // namespace basil

#endif  // SRC_UTILITY_METRICSREPORTER_HPP_
