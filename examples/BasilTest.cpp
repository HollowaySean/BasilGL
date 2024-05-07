#include <Basil/App.hpp>
#include <Basil/Data.hpp>
#include <Basil/Process.hpp>
#include <Basil/Utility.hpp>
#include <Basil/Window.hpp>

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    auto fragmentPath =
        std::filesystem::path(EXAMPLE_DIR) / "shaders/test.frag";
    auto jsonPath =
        std::filesystem::path(EXAMPLE_DIR) / "assets/test.json";

    auto window = basil::WindowView::Builder()
            .withTitle("My window")
            .withDimensions(400, 400)
            .withTopPane(basil::HotReloadShaderPane::Builder()
                .fromFilePath(fragmentPath)
                .build())
            .build();

    auto basilApp = basil::BasilApp::Builder()
        .withWindow(window)
        .withController(basil::ProcessController::Builder()
            .withFrameCap(30)
            .withEarlyProcess(basil::UniformJSONFileWatcher::Builder()
                .withFilePath(jsonPath)
                .withPublisher(window)
                .build())
            .build())
        .build();

    basilApp->run();

    return 0;
}
