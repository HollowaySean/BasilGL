#include <filesystem>

#include <Basil/App.hpp>

#include "ShadertoyImGuiPane.hpp"

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    auto currentPath = std::filesystem::path(__FILE__).parent_path();
    auto shaderPath = currentPath / "shaders/test.shadertoy";
    auto jsonPath =   currentPath /  "assets/test.json";

    auto screenshotPath = std::filesystem::path(BUILD_DIR)
        / "output/screenshots";
    if (!std::filesystem::exists(screenshotPath)) {
        std::filesystem::create_directories(screenshotPath);
    }

    // TODO(sholloway): Fix iResolution not being specific to pane

    std::shared_ptr<basil::IPane> screenshotFocus;
    auto basilApp = basil::BasilApp::Builder()
        .withController(basil::ProcessController::Builder()
            .withFrameCap(60)
            .build())
        .withWidget(basil::WindowView::Builder()
            .withTitle("BasilGL Shadertoy Demo")
            .withDimensions(1000, 750)
            .withTopPane(basil::SplitPane::Builder()
                .withFirstPane(
                    screenshotFocus =
                        basil::HotReloadShaderPane::Builder()
                    .fromFilePath(shaderPath)
                    .build())
                .withSecondPane(ShadertoyImGuiPane::Builder()
                    .build())
                .withPaneExtentInPixels(200)
                .withFixedPane(basil::SplitPaneFixedPane::SECOND)
                .build())
            .build())
        .withWidget(basil::UniformJSONFileWatcher::Builder()
            .withFilePath(jsonPath)
            .build())
        .withWidget(basil::MetricsReporter::Builder()
            .withRegularity(300)
            .withLogLevel(basil::LogLevel::INFO)
            .build())
        .withWidget(basil::ShadertoyUniformPublisher::Builder()
            .build())
        .withWidget(basil::ScreenshotTool::Builder()
            .withTriggerKey(GLFW_KEY_S)
            .withSaveDirectory(screenshotPath)
            .withSaveFileName("image_{index}_{time:%y%m%d_%H%M%S}.png")
            .withFocusPane(screenshotFocus)
            .build())
        .build();

    basilApp->run();

    return 0;
}
