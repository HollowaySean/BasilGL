#include "MetricsReporter.hpp"

namespace basil {

MetricsReporter::MetricsReporter() {
    // Set process name for IProcess
    IProcess::setProcessName("MetricsReporter");
}

void MetricsReporter::onLoop() {
    if (controller == nullptr) return;

    MetricsObserver& metrics = controller->getMetricsObserver();
    MetricsRecord record = metrics.getCurrentMetrics();

    if (record.frameID % regularity == 0
            && record.frameID > 0) {
        logger.lineBreak(logLevel);

        logger.log(
            fmt::format("Frame #: {}", record.frameID),
            logLevel);

        logger.log(
            fmt::format("Frame rate: {:.2f}", record.getFrameRate()),
            logLevel);

        logger.log(
            fmt::format("Max frame rate: {:.2f}", record.getUncappedFrameRate()),
            logLevel);

        for (auto process : record.processTimes) {
            std::string& name = process.first->processName;
            auto timeInNanoseconds = std::chrono::nanoseconds(process.second);

            double timeInMilliseconds = timeInNanoseconds.count() / 1'000'000.;

            logger.log(
                fmt::format("Process \'{}\': {:.3f}ms", name, timeInMilliseconds),
                logLevel);
        }
    }
}

}  // namespace basil
