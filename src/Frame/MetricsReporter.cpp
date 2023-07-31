#include "MetricsReporter.hpp"

#include <stdio.h>

MetricsReporter::MetricsReporter(
    FrameMetrics *metricsObserver, int regularity):
    metrics(metricsObserver), regularity(regularity) {}

void MetricsReporter::onLoop() {
    FrameMetricsRecord record =
        metrics->getExcludingCurrent();

    if (record.frameID % regularity == 0
            && record.frameID > 0) {
        printf("Frame #: %d\n", record.frameID);
        printf("Frame rate: %.2f\n", record.getFrameRate());
        printf("Max frame rate: %.2f\n", record.getUncappedFrameRate());
    }
}
