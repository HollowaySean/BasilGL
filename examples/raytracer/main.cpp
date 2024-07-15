#include <Basil/App.hpp>

#include "src/CameraController.hpp"
#include "src/SidePanel.hpp"

namespace rt = basil::raytracer;

struct Sphere {         // Alignment    // Offset
    float position[3];  // 16           // 0
    float size;         // 4            // 16

    static inline unsigned int ALIGNMENT = 20;
};

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

    // TODO(sholloway): Parameterized spheres
    // TODO(sholloway): More advanced rendering
    // TODO(sholloway): Stats and controls
    // TODO(sholloway): More info text

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
                .withPaneExtentInPixels(1)
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
            .build())
        .build();

    const unsigned int NUM_SPHERES = 10;
    std::vector<float> spherePositions
        = std::vector<float>();
    std::vector<float> sphereSizes
        = std::vector<float>();

    for (int i = 0; i < NUM_SPHERES; i++) {
        spherePositions.push_back(2*i - NUM_SPHERES + 1);
        spherePositions.push_back(1.0f);
        spherePositions.push_back(10.0f);
        sphereSizes.push_back(static_cast<float>(i + 1) / NUM_SPHERES);
    }
    auto positionUniform = std::make_shared<basil::GLUniformVector<float>>(
        spherePositions, "spherePositions", 3, 1, NUM_SPHERES);
    auto sizeUniform = std::make_shared<basil::GLUniformVector<float>>(
        sphereSizes, "sphereSizes", 1, 1, NUM_SPHERES);

    auto sphereUniforms = basil::ShaderUniformModel();
    sphereUniforms.addUniform(positionUniform);
    sphereUniforms.addUniform(sizeUniform);
    sphereUniforms.addUniform(NUM_SPHERES, "numSpheres");

    basilApp->sendMessage(basil::DataMessage(sphereUniforms));

    basilApp->run();

    return 0;
}
