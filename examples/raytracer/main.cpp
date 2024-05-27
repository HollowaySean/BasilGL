#include <Basil/App.hpp>

#include "CameraController.hpp"

#include "SidePanel.hpp"

/**
 * @brief BasilGL example project TODO(sholloway): This
 */
int main(int argc, char** argv) {
    auto currentPath = std::filesystem::path(__FILE__).parent_path();
    auto shaderPath = currentPath / "shaders/raytracer.frag";

    auto basilApp = basil::BasilApp::Builder()
        .withController(basil::ProcessController::Builder()
            .build())
        .withWidget(basil::WindowView::Builder()
            .withDimensions(800, 600)
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
        .withWidget(CameraController::Builder()
            .build())
        .build();

    basilApp->run();

    return 0;
}
