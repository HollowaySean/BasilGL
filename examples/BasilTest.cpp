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

    #if BASIL_INCLUDE_IMGUI
    class MyImGuiPane : public basil::ImGuiPane,
                        public basil::IBuildable<MyImGuiPane> {
     public:
        const void drawImGuiContent() override {
            ImGui::Text("Child of ImGuiPane!");
        }
    };
    #endif

    auto basilApp = basil::BasilApp::Builder()
        .withController(basil::ProcessController::Builder()
            .withFrameCap(60)
            .build())
        .withWidget(basil::WindowView::Builder()
            .withTitle("My window")
            .withDimensions(400, 400)
            .withTopPane(basil::SplitPane::Builder()
                .withFirstPane(basil::HotReloadShaderPane::Builder()
                    .fromFilePath(fragmentPath)
                    .build())

                #if BASIL_INCLUDE_IMGUI
                .withSecondPane(MyImGuiPane::Builder()
                    .build())
                #endif

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
        .build();

    basilApp->run();

    return 0;
}
