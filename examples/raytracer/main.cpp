#include <Basil/App.hpp>

#include "src/CubemapLoader.hpp"
#include "src/CameraController.hpp"
#include "src/SidePanel.hpp"

namespace rt = basil::raytracer;

/**
 * @brief BasilGL example project TODO(sholloway): This
 */
int main(int argc, char** argv) {
    auto currentPath = std::filesystem::path(__FILE__).parent_path();
    auto shaderPath = currentPath / "shaders/raytracer.frag";
    auto skyboxPath = currentPath / "assets/skybox";

    basil::Logger::get().setLevel(basil::LogLevel::DEBUG);

    // TODO(sholloway): Parameterized spheres
    // TODO(sholloway): More advanced rendering
    // TODO(sholloway): Cubemap loading from JSON
    // TODO(sholloway): Vector loading from JSON
    // TODO(sholloway): Clean up src files
    // TODO(sholloway): Stats and controls
    // TODO(sholloway): Screenshot tool

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
                .withFirstPane(basil::HotReloadShaderPane::Builder()
                    .fromFilePath(shaderPath)
                    .build())
                .withSecondPane(rt::SidePanel::Builder().build())
                .build())
            .build())
        .withWidget(basil::MetricsReporter::Builder()
            .withRegularity(300)
            .withLogLevel(basil::LogLevel::INFO)
            .build())
        .withWidget(rt::CameraController::Builder()
            .build())
        .withWidget(std::make_shared<rt::CubemapLoader>(skyboxPath, "skybox"))
        .build();

    basilApp->run();

    return 0;
}
