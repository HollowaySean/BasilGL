#include "Frame.hpp"
#include "Window.hpp"

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    FrameController frameController = FrameController();

    FrameMetrics *frameMetrics = &frameController.metrics;
    frameMetrics->setBufferSize(20);

    MetricsReporter reporter = MetricsReporter(
        frameMetrics, frameMetrics->getBufferSize());
    frameController.addProcess(&reporter);

    WindowView windowView = WindowView();
    frameController.addProcess(&windowView);

    frameController.setFrameCap(20);
    frameController.run();

    return 0;
}
