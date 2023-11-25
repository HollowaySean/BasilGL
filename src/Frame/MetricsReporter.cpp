#include "MetricsReporter.hpp"

MetricsReporter::MetricsReporter(
        FrameMetrics *metricsObserver, int regularity,
            Level logLevel, std::ostream& ostream):
        metrics(metricsObserver), regularity(regularity),
            processNames(), logLevel(logLevel), ostream(ostream) {
    processNames = metricsObserver->getProcessNames();
}

void MetricsReporter::onLoop() {
    FrameMetricsRecord record =
        metrics->getExcludingCurrent();

    if (record.frameID % regularity == 0
            && record.frameID > 0) {
        logger.lineBreak(logLevel, ostream);

        logger.log(std::format(FRAME_FORMAT,
            static_cast<int>(record.frameID)),
            logLevel, ostream);
        logger.log(std::format(FRAME_RATE_FORMAT,
            record.getFrameRate()),
            logLevel, ostream);
        logger.log(std::format(MAX_FRAME_RATE_FORMAT,
            record.getUncappedFrameRate()),
            logLevel, ostream);

        for (auto process : record.processTime) {
            std::string processName = "anonymous";
            if (processNames->count(process.first) > 0) {
                processName = (*processNames)[process.first];
            }
            logger.log(std::format(PROCESS_TIME_FORMAT,
                processName.c_str(), 1000. * process.second),
                logLevel, ostream);
        }
    }
}
