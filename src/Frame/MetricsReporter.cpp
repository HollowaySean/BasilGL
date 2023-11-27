#include "MetricsReporter.hpp"

MetricsReporter::MetricsReporter(
        FrameMetrics *metricsObserver, int regularity,
            Level logLevel):
        metrics(metricsObserver), regularity(regularity),
            processNames(), logLevel(logLevel) {
    processNames = metricsObserver->getProcessNames();
}

void MetricsReporter::onLoop() {
    FrameMetricsRecord record =
        metrics->getExcludingCurrent();

    if (record.frameID % regularity == 0
            && record.frameID > 0) {
        logger.lineBreak(logLevel);

        logger.log(fmt::format(FRAME_FORMAT,
            static_cast<int>(record.frameID)),
            logLevel);
        logger.log(fmt::format(FRAME_RATE_FORMAT,
            record.getFrameRate()),
            logLevel);
        logger.log(fmt::format(MAX_FRAME_RATE_FORMAT,
            record.getUncappedFrameRate()),
            logLevel);

        for (auto process : record.processTime) {
            std::string processName = "anonymous";
            if (processNames->count(process.first) > 0) {
                processName = (*processNames)[process.first];
            }
            logger.log(fmt::format(PROCESS_TIME_FORMAT,
                processName.c_str(), 1000. * process.second),
                logLevel);
        }
    }
}
