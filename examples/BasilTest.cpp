#include <Basil/App.hpp>
#include <Basil/Widget.hpp>

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    auto exPath = std::filesystem::path(EXAMPLE_DIR);
    auto fragmentPath = exPath / "shaders/test.shadertoy";
    auto jsonPath =     exPath / "assets/test.json";

    // TODO(sholloway): Easy way to inject uniforms
    auto basilApp = basil::BasilApp::Builder()
        .withWindow(basil::WindowView::Builder()
            .withTitle("My window")
            .withDimensions(400, 400)
            .withTopPane(basil::HotReloadShaderPane::Builder()
                .fromFilePath(fragmentPath)
                .build())
            .build())
        .withController(basil::ProcessController::Builder()
            .withFrameCap(60)
            .build())
        .withWidget(basil::UniformJSONFileWatcher::Builder()
            .withFilePath(jsonPath)
            .build())
        .withWidget(basil::MetricsReporter::Builder()
            .withRegularity(120)
            .withLogLevel(basil::LogLevel::INFO)
            .build())
        .withWidget(std::make_shared<basil::ShadertoyUniformPublisher>())
        .build();

    basilApp->run();

    return 0;
}
