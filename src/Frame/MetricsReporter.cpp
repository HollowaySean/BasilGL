#include "MetricsReporter.hpp"

#include <stdio.h>

MetricsReporter::MetricsReporter(
        FrameMetrics *metricsObserver, int regularity):
        metrics(metricsObserver), regularity(regularity), processNames() {
    processNames = metricsObserver->getProcessNames();
}

void MetricsReporter::onLoop() {
    FrameMetricsRecord record =
        metrics->getExcludingCurrent();

    if (record.frameID % regularity == 1
            && record.frameID > 1) {
        printf("\nFrame #: %d\n", record.frameID - 1);
        printf("Frame rate: %.2f\n", record.getFrameRate());
        printf("Max frame rate: %.2f\n", record.getUncappedFrameRate());

        for (auto process : record.processTime) {
            std::string processName = "anonymous";
            if (processNames->count(process.first) > 0) {
                processName = (*processNames)[process.first];
            }
            printf("Process \'%s\': %fms\n",
                processName.c_str(), 1000. * process.second);
        }
    }
}
