#include <Basil/Frame.hpp>
#include <Basil/Window.hpp>

using basil::FrameController;
using basil::FrameMetrics;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLTexturePane;
using basil::PaneProps;
using basil::WindowView;
using basil::MetricsReporter;

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    FrameController frameController = FrameController();

    FrameMetrics *frameMetrics = &frameController.metrics;
    frameMetrics->setBufferSize(20);

    MetricsReporter reporter = MetricsReporter(
        frameMetrics, frameMetrics->getBufferSize());
    frameController.addProcess(&reporter,
        FrameController::Privilege::NONE,
        "MetricsReporter");

    WindowView windowView = WindowView();
    frameController.addProcess(&windowView,
        FrameController::Privilege::HIGH,
        "WindowView");


    std::filesystem::path fragmentPath =
        std::filesystem::path(SOURCE_DIR) / "Window/shaders/default.frag";

    PaneProps paneProps = windowView.getTopPaneProps();
    GLVertexShader noOpVertex = GLVertexShader::noOpShader();

    std::shared_ptr<GLVertexShader> vertexShader =
        std::shared_ptr<GLVertexShader>(&noOpVertex);
    std::shared_ptr<GLFragmentShader> fragmentShader =
        std::make_shared<GLFragmentShader>(fragmentPath);
    std::shared_ptr<GLShaderProgram> shaderProgram =
        std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);

    std::shared_ptr<GLTexturePane> topPane =
        std::make_shared<GLTexturePane>(paneProps, shaderProgram);

    windowView.setTopPane(topPane);

    frameController.setFrameCap(30);
    frameController.run();

    return 0;
}
