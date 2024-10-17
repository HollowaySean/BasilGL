#include <random>

#include <Basil/App.hpp>

#include "src/CameraController.hpp"
#include "src/SidePanel.hpp"
#include "src/SphereGenerator.hpp"

namespace rt = basil::raytracer;

/**
 * @brief BasilGL example project demonstrating a simple ray tracer
 * built with BasilApp & BasilWidget classes.
 *
 * Current features:
 * - Hot reloading for shader & uniform files
 * - Mouse and keyboard input handling
 * - Screen capture to file
 * - Metrics reporting to console
 * - ImGui information panel
 *
 * Features to be implemented:
 * - Setting uniforms from ImGui panel
 * - Metrics reporting to ImGui
 * - Demo vs. render settings
 */
int main(int argc, char** argv) {
    auto currentPath = std::filesystem::path(__FILE__).parent_path();
    auto shaderPath = currentPath / "shaders/raytracer.frag";
    auto jsonPath = currentPath / "assets/uniforms.json";

    auto screenshotPath = currentPath / "output";
    if (!std::filesystem::is_directory(screenshotPath)) {
        std::filesystem::create_directory(screenshotPath);
    }

    basil::Logger& logger = basil::Logger::get();
    logger.setLevel(basil::LogLevel::DEBUG);

    std::shared_ptr<basil::IPane> focusPane;
    auto basilApp = basil::BasilApp::Builder()
        .withController(basil::ProcessController::Builder()
            .withFrameCap(60)
            .build())
        .withWidget(basil::WindowView::Builder()
            .withDimensions(1100, 800)
            .withTitle("BasilGL Ray Tracing Demo")
            .withTopPane(basil::SplitPane::Builder()
                .withFixedPane(basil::SplitPane::FixedPane::SECOND)
                .withPaneExtentInPixels(300)
                .withFirstPane(focusPane = basil::HotReloadShaderPane::Builder()
                    .fromFilePath(shaderPath)
                    .build())
                .withSecondPane(rt::SidePanel::Builder().build())
                .build())
            .build())
        .withWidget(basil::MetricsReporter::Builder()
            .withRegularity(300)
            .withLogLevel(basil::LogLevel::INFO)
            .build())
        .withWidget(basil::UniformJSONFileWatcher::Builder()
            .withFilePath(jsonPath)
            .build())
        .withWidget(basil::ScreenshotTool::Builder()
            .withFocusPane(focusPane)
            .withSaveDirectory(screenshotPath)
            .withSaveFileName("screenshot.png")
            .withTriggerKey(GLFW_KEY_F)
            .build())
        .withWidget(rt::CameraController::Builder()
            .withFocusPane(focusPane)
            .withPosition(0.0f, 5.0f, -10.0f)
            .withTiltAngle(-30.0f)
            .withMaximumTilt(80.0f)
            .build())
        .withWidget(rt::SphereGenerator::Builder()
            .withGridSize(4)
            .build())
        .build();

    basilApp->run();

    return 0;
}
