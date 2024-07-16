#include <random>

#include <Basil/App.hpp>

#include "src/CameraController.hpp"
#include "src/SidePanel.hpp"
#include "src/SphereGenerator.hpp"

namespace rt = basil::raytracer;

/**
 * @brief BasilGL example project TODO(sholloway): This
 */
int main(int argc, char** argv) {
    auto currentPath = std::filesystem::path(__FILE__).parent_path();
    auto shaderPath = currentPath / "shaders/raytracer.frag";
    auto jsonPath = currentPath / "assets/uniforms.json";

    auto screenshotPath = currentPath / "output";
    if (!std::filesystem::is_directory(screenshotPath)) {
        std::filesystem::create_directory(screenshotPath);
    }

    basil::Logger::get().setLevel(basil::LogLevel::DEBUG);

    // TODO(sholloway): Preview vs render settings?
    // TODO(sholloway): Stats and controls
    // TODO(sholloway): More info text
    // TODO(sholloway): Copious documentation

    std::shared_ptr<basil::IPane> focusPane;
    auto basilApp = basil::BasilApp::Builder()
        .withController(basil::ProcessController::Builder()
            .withFrameCap(60)
            .build())
        .withWidget(basil::WindowView::Builder()
            .withDimensions(1000, 800)
            .withTitle("BasilGL Ray Tracing Demo")
            .withTopPane(basil::SplitPane::Builder()
                .withFixedPane(basil::SplitPane::FixedPane::SECOND)
                .withPaneExtentInPixels(200)
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
            .withGridSize(8)
            .build())
        .build();

    basilApp->run();

    return 0;
}
