#include <filesystem>

#include <Basil/App.hpp>

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    auto exPath = std::filesystem::path(EXAMPLE_DIR);
    auto fragmentPath = exPath / "shaders/test.frag";
    auto jsonPath =     exPath / "assets/test.json";

    class MyImGuiPane : public basil::ImGuiPane,
                        public basil::IBuildable<MyImGuiPane> {
     public:
        const void drawImGuiContent() override {
            ImGui::Text("Child of ImGuiPane!");
        }
    };

    std::shared_ptr<basil::IPane> screenshotFocus;
    auto basilApp = basil::BasilApp::Builder()
        .withController(basil::ProcessController::Builder()
            .withFrameCap(0)
            .build())
        .withWidget(basil::WindowView::Builder()
            .withTitle("My window")
            .withDimensions(600, 400)
            .withTopPane(basil::SplitPane::Builder()
                .withFirstPane(screenshotFocus =
                    basil::HotReloadShaderPane::Builder()
                    .fromFilePath(fragmentPath)
                    .build())
                .withSecondPane(MyImGuiPane::Builder()
                    .build())
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
            .withSaveDirectory(exPath / "../build/screenshots")
            .withSaveFileName("image_{index}_{time:%y%m%d_%H%M%S}.png")
            .withFocusPane(screenshotFocus)
            .build())
        .build();

    basilApp->run();

    return 0;
}
