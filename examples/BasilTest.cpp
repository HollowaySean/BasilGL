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
using basil::GLTexture3D;
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
    auto jsonPath =
        std::filesystem::path(EXAMPLE_DIR) / "assets/test.json";
    auto dataModel = basil::FileDataLoader::modelFromJSON(jsonPath);

    auto fragmentPath =
        std::filesystem::path(EXAMPLE_DIR) / "shaders/test.frag";

    auto window = WindowView::Builder()
            .withTitle("My window")
            .withDimensions(400, 400)
            .withTopPane(HotReloadShaderPane::Builder()
                .fromFilePath(fragmentPath)
                .build())
            .build();

    if (dataModel.has_value()) {
        window->publishData(dataModel.value());
    }

    auto basilApp = BasilApp::Builder()
        .withWindow(window)
        .withController(ProcessController::Builder()
            .withFrameCap(30)
            .build())
        .build();

    basilApp->run();

    return 0;
}
