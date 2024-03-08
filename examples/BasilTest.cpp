#include <Basil/App.hpp>
#include <Basil/Process.hpp>
#include <Basil/Utility.hpp>
#include <Basil/Window.hpp>

using basil::BasilApp;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLShaderPane;
using basil::LogLevel;
using basil::MetricsReporter;
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

    auto basilApp = BasilApp::Builder()
        .withWindow(WindowView::Builder()
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
            .build())
        .withController(ProcessController::Builder()
            .withFrameCap(30)
            .withLateProcess(MetricsReporter::Builder()
                .withLogLevel(LogLevel::INFO)
                .withRegularity(30)
                .build())
            .build())
        .build();

    basilApp->run();

    return 0;
}
