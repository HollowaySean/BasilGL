#include <Basil/App.hpp>

#include "CubemapLoader.hpp"
#include "CameraController.hpp"
#include "SidePanel.hpp"

/**
 * @brief BasilGL example project TODO(sholloway): This
 */
int main(int argc, char** argv) {
    auto currentPath = std::filesystem::path(__FILE__).parent_path();
    auto shaderPath = currentPath / "shaders/raytracer.frag";
    auto skyboxPath = currentPath / "assets/skybox";

    basil::Logger::get().setLevel(basil::LogLevel::DEBUG);

    // TODO(sholloway): Keyboard camera movement
    // TODO(sholloway): Mouse camera rotation
    // TODO(sholloway): Parameterized spheres
    // TODO(sholloway): More advanced rendering
    // TODO(sholloway): Cubemap loading from JSON
    // TODO(sholloway): Vector loading from JSON
    // TODO(sholloway): Move files to src folder
    // TODO(sholloway): Stats and controls
    // TODO(sholloway): Screenshot tool

    auto basilApp = basil::BasilApp::Builder()
        .withController(basil::ProcessController::Builder()
            .withFrameCap(60)
            .build())
        .withWidget(basil::WindowView::Builder()
            .withDimensions(1200, 1000)
            .withTitle("BasilGL Ray Tracing Demo")
            .withTopPane(basil::SplitPane::Builder()
                .withFixedPane(basil::SplitPane::FixedPane::FIRST)
                .withPaneAspectRatio(1.0)
                .withFirstPane(basil::HotReloadShaderPane::Builder()
                    .fromFilePath(shaderPath)
                    .build())
                .withSecondPane(SidePanel::Builder().build())
                .build())
            .build())
        .withWidget(basil::MetricsReporter::Builder()
            .withRegularity(300)
            .withLogLevel(basil::LogLevel::INFO)
            .build())
        .withWidget(CameraController::Builder()
            .build())
        .withWidget(std::make_shared<CubemapLoader>(skyboxPath, "skybox"))
        .build();

    basilApp->run();

    return 0;
}
