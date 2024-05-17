#pragma once

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/App.hpp>

namespace basil {

/** @brief Utility FrameProcess that prints frame rate to
 *  the command line.
*/
class MetricsReporter : public IBasilWidget,
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

    LOGGER_FORMAT LOG_FRAME =
        "Frame #: {}";
    LOGGER_FORMAT LOG_FRAME_RATE =
        "Frame rate: {:.2f}";
    LOGGER_FORMAT LOG_MAX_FRAME_RATE =
        "Max frame rate: {:.2f}";
    LOGGER_FORMAT LOG_PROCESS_TIME =
        "Process \'{}\': {:.3f}ms";
};

}   // namespace basil
