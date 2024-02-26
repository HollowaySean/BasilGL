#include <Basil/Application.hpp>

using basil::BasilApp;

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    std::filesystem::path fragmentPath =
        std::filesystem::path(SOURCE_DIR) / "Window/shaders/default.frag";

    BasilApp app = BasilApp::Builder()
        .withShaderPane(fragmentPath)
        .withFrameRateCap(30)
        .withMetricsReporter(20)
        .build();

    app.run();

    return 0;
}
