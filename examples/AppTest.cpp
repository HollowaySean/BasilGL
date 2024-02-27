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
    std::filesystem::path vertexPath =
        std::filesystem::path(SOURCE_DIR) / "Window/shaders/default.vert";

    BasilApp app = BasilApp::Builder()
        .withShaderPane(vertexPath, fragmentPath)
        .withFrameRateCap(30)
        .build();

    app.run();

    return 0;
}
