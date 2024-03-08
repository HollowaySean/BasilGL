#include <Basil/Process.hpp>
#include <Basil/Window.hpp>

using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLShaderPane;
using basil::MetricsReporter;
using basil::PaneProps;
using basil::PaneOrientation;
using basil::ProcessController;
using basil::ProcessPrivilege;
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
    auto fragmentPath =
        std::filesystem::path(SOURCE_DIR) / "Window/shaders/default.frag";

    auto windowView = WindowView::Builder()
        .withTitle("My window")
        .withDimensions(500, 500)
        .withTopPane(SplitPane::Builder()
            .withFirstPane(GLShaderPane::Builder()
                .fromShaderFile(fragmentPath)
                .build())
            .withSecondPane(GLShaderPane::Builder()
                .fromShaderFile(fragmentPath)
                .build())
            .withOrientation(PaneOrientation::VERTICAL)
            .withGapWidth(5)
            .withPaneExtentInPercent(66.6)
            .build())
        .build();

    auto metricsReporter = std::make_shared<MetricsReporter>();
    metricsReporter->setRegularity(30);

    auto controller = ProcessController::Builder()
        .withFrameCap(30)
        .withProcess(std::move(windowView), ProcessPrivilege::HIGH)
        .withLateProcess(metricsReporter)
        .build();

    controller->run();

    return 0;
}
