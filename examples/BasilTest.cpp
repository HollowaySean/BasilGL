#include <Basil/Frame.hpp>
#include <Basil/Window.hpp>

using basil::FrameController;
using basil::FrameMetrics;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
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

    auto fragmentPath =
        std::filesystem::path(SOURCE_DIR) / "Window/shaders/default.frag";

    auto shaderProgram = GLShaderProgram::Builder()
        .withFragmentShader(fragmentPath)
        .withDefaultVertexShader()
        .build();

    // TODO(sholloway): Default GLTexturePane or optional PaneProps
    PaneProps paneProps = PaneProps {
        .width = 500,
        .height = 500,
        .xOffset = 0,
        .yOffset = 0
    };

    auto topPane =
        std::make_shared<GLTexturePane>(PaneProps(), std::move(shaderProgram));

    auto windowView = WindowView::Builder()
        .withTitle("My window")
        .withDimensions(500, 500)
        .withTopPane(topPane)
        .build();

    frameController.addProcess(windowView.get(),
        FrameController::Privilege::HIGH,
        "WindowView");

    frameController.setFrameCap(30);
    frameController.run();

    return 0;
}
