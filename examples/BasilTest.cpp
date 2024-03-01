#include <Basil/Frame.hpp>
#include <Basil/Window.hpp>

using basil::FrameController;
using basil::FrameMetrics;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLShaderPane;
using basil::MetricsReporter;
using basil::PaneProps;
using basil::PaneOrientation;
using basil::SplitPane;
using basil::WindowView;

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

    auto topPane =
        std::make_shared<GLShaderPane>(PaneProps(), std::move(shaderProgram));


    auto secondProgram = GLShaderProgram::Builder()
        .withFragmentShader(fragmentPath)
        .withDefaultVertexShader()
        .build();
    auto secondPane =
        std::make_shared<GLShaderPane>(PaneProps(), std::move(secondProgram));

    auto windowView = WindowView::Builder()
        .withTitle("My window")
        .withDimensions(500, 500)
        .withTopPane(SplitPane::Builder()
            .withFirstPane(topPane)
            .withSecondPane(secondPane)
            .withOrientation(PaneOrientation::VERTICAL)
            .withGapWidth(5)
            .withPaneExtentInPercent(66.6)
            .build())
        .build();

    frameController.addProcess(windowView.get(),
        FrameController::Privilege::HIGH,
        "WindowView");

    frameController.setFrameCap(30);
    frameController.run();

    return 0;
}
