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

    auto screenshot = std::make_shared<basil::ScreenshotTool>();
    screenshot->setTriggerKey(GLFW_KEY_S);
    screenshot->setSaveDirectory(exPath / "../build/screenshots");

    auto basilApp = basil::BasilApp::Builder()
        .withController(basil::ProcessController::Builder()
            .withFrameCap(1)
            .build())
        .withWidget(basil::WindowView::Builder()
            .withTitle("My window")
            .withDimensions(1000, 1000)
            .withTopPane(basil::SplitPane::Builder()
                .withFirstPane(basil::HotReloadShaderPane::Builder()
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
            .withRegularity(1)
            .withLogLevel(basil::LogLevel::INFO)
            .build())
        .withWidget(basil::ShadertoyUniformPublisher::Builder()
            .build())
        .withWidget(screenshot)
        .build();

    basilApp->run();

    return 0;
}
