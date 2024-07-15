#include <random>

#include <Basil/App.hpp>

#include "src/CameraController.hpp"
#include "src/SidePanel.hpp"

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
            .withTopPane(focusPane = basil::HotReloadShaderPane::Builder()
                .fromFilePath(shaderPath)
                .build())
            .build())
            // .withTopPane(basil::SplitPane::Builder()
            //     .withFixedPane(basil::SplitPane::FixedPane::SECOND)
            //     .withPaneExtentInPixels(200)
            //     .withFirstPane(focusPane = basil::HotReloadShaderPane::Builder()
            //         .fromFilePath(shaderPath)
            //         .build())
            //     .withSecondPane(rt::SidePanel::Builder().build())
            //     .build())
            // .build())
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
        .build();

    const int SPHERE_GRID_SIZE = 5;
    const int NUM_SPHERES = SPHERE_GRID_SIZE * SPHERE_GRID_SIZE;
    std::vector<float> spherePositions = std::vector<float>();
    std::vector<float> sphereSizes = std::vector<float>();
    std::vector<float> sphereAlbedo = std::vector<float>();
    std::vector<float> sphereSpecular = std::vector<float>();

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randomOffset(-0.5f, 0.5f);
    std::uniform_real_distribution<float> randomSize(0.2f, 1.0f);
    std::uniform_real_distribution<float> randomColor(0.0f, 1.0f);
    std::uniform_real_distribution<float> randomSpecular(0.0f, 0.4f);
    for (int x = 0; x < SPHERE_GRID_SIZE; x++) {
        for (int z = 0; z < SPHERE_GRID_SIZE; z++) {
            float xPos = 2*x - SPHERE_GRID_SIZE + 1 + randomOffset(generator);
            float yPos = 1.0f + randomOffset(generator);
            float zPos = 2*z + randomOffset(generator);

            spherePositions.push_back(xPos);
            spherePositions.push_back(yPos);
            spherePositions.push_back(zPos);
            sphereSizes.push_back(randomSize(generator));
            sphereAlbedo.push_back(randomColor(generator));
            sphereAlbedo.push_back(randomColor(generator));
            sphereAlbedo.push_back(randomColor(generator));
            sphereSpecular.push_back(randomSpecular(generator));
            sphereSpecular.push_back(randomSpecular(generator));
            sphereSpecular.push_back(randomSpecular(generator));
        }
    }

    auto positionUniform = std::make_shared<basil::GLUniformVector<float>>(
        spherePositions, "spherePositions", 3, 1, NUM_SPHERES);
    auto sizeUniform = std::make_shared<basil::GLUniformVector<float>>(
        sphereSizes, "sphereSizes", 1, 1, NUM_SPHERES);
    auto albedoUniform = std::make_shared<basil::GLUniformVector<float>>(
        sphereAlbedo, "sphereAlbedo", 3, 1, NUM_SPHERES);
    auto specularUniform = std::make_shared<basil::GLUniformVector<float>>(
        sphereSpecular, "sphereSpecular", 3, 1, NUM_SPHERES);

    auto sphereUniforms = basil::ShaderUniformModel::Builder()
        .withUniform(positionUniform)
        .withUniform(sizeUniform)
        .withUniform(albedoUniform)
        .withUniform(specularUniform)
        .withUniform(NUM_SPHERES, "numSpheres")
        .build();

    basilApp->sendMessage(basil::DataMessage(sphereUniforms));

    basilApp->run();

    return 0;
}
