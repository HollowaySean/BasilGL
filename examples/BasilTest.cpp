#include <Basil/App.hpp>
#include <Basil/Utility.hpp>

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
    auto jsonPath =
        std::filesystem::path(EXAMPLE_DIR) / "assets/test.json";

    auto basilApp = basil::BasilApp::Builder()
        .withWindow(basil::WindowView::Builder()
            .withTitle("My window")
            .withDimensions(400, 400)
            .withTopPane(basil::HotReloadShaderPane::Builder()
                .fromFilePath(fragmentPath)
                .build())
            .build())
        .withController(basil::ProcessController::Builder()
            .withFrameCap(30)
            .build())
        .withWidget(basil::UniformJSONFileWatcher::Builder()
            .withFilePath(jsonPath)
            .build())
        .withWidget(basil::MetricsReporter::Builder()
            .withRegularity(120)
            .withLogLevel(basil::LogLevel::INFO)
            .build())
        .build();

    basilApp->run();

    return 0;
}
