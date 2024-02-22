#include <Basil/Frame.hpp>
#include <Basil/Window.hpp>

using basil::FrameController;
using basil::FrameMetrics;
using basil::GLTexturePane;
using basil::PaneProps;
using basil::WindowView;
using basil::MetricsReporter;

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
    frameController.addProcess(&reporter,
        FrameController::Privilege::NONE,
        "MetricsReporter");

    WindowView windowView = WindowView();
    frameController.addProcess(&windowView,
        FrameController::Privilege::HIGH,
        "WindowView");


    std::filesystem::path fragmentPath =
        std::filesystem::path(SOURCE_DIR) / "Window/shaders/default.frag";

    PaneProps paneProps = windowView.getTopPaneProps();
    GLTexturePane topPane = GLTexturePane(paneProps, fragmentPath);

    windowView.setTopPane(&topPane);

    frameController.setFrameCap(30);
    frameController.run();

    return 0;
}
