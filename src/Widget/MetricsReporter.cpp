#include "MetricsReporter.hpp"

namespace basil {

MetricsReporter::MetricsReporter() : IBasilWidget(
    ProcessOrdinal::LATE,
    ProcessPrivilege::NONE,
    "MetricsReporter") {}

void MetricsReporter::onLoop() {
    if (controller == nullptr) return;

    MetricsObserver& metrics = controller->getMetricsObserver();
    MetricsRecord record = metrics.getCurrentMetrics();

    if (record.frameID % regularity == 0
            && record.frameID > 0) {
        logger.lineBreak(logLevel);

        logger.log(
            fmt::format(LOG_FRAME,
                record.frameID),
            logLevel);

        logger.log(
            fmt::format(LOG_FRAME_RATE,
                record.getFrameRate()),
            logLevel);

        logger.log(
            fmt::format(LOG_MAX_FRAME_RATE,
                record.getUncappedFrameRate()),
            logLevel);

        for (auto process : record.processTimes) {
            std::string& name = process.first->processName;
            auto timeInNanoseconds = std::chrono::nanoseconds(process.second);

            double timeInMilliseconds = timeInNanoseconds.count() / 1'000'000.;

            logger.log(
                fmt::format(LOG_PROCESS_TIME,
                    name, timeInMilliseconds),
                logLevel);
        }
    }
}

MetricsReporter::Builder&
MetricsReporter::Builder::withRegularity(unsigned int regularity) {
    impl->setRegularity(regularity);
    return *this;
}

MetricsReporter::Builder&
MetricsReporter::Builder::withLogLevel(LogLevel level) {
    impl->setLogLevel(level);
    return *this;
}

}  // namespace basil
