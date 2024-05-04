#include <Basil/App.hpp>
#include <Basil/Data.hpp>
#include <Basil/Process.hpp>
#include <Basil/Utility.hpp>
#include <Basil/Window.hpp>

using basil::BasilApp;
using basil::GLShaderPane;
using basil::HotReloadShaderPane;
using basil::GLShaderProgram;
using basil::GLTexture2D;
using basil::LogLevel;
using basil::MetricsReporter;
using basil::ProcessController;
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
        std::filesystem::path(EXAMPLE_DIR) / "shaders/test.frag";

    auto basilApp = BasilApp::Builder()
        .withWindow(WindowView::Builder()
            .withTitle("My window")
            .withDimensions(400, 400)
            .withTopPane(HotReloadShaderPane::Builder()
                .fromFilePath(fragmentPath)
                .build())
                    .build())
        .withController(ProcessController::Builder()
            .withFrameCap(30)
            .build())
        .build();

    basilApp->run();

    return 0;
}
