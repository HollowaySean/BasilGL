#include <Basil/App.hpp>
#include <Basil/Data.hpp>
#include <Basil/Process.hpp>
#include <Basil/Utility.hpp>
#include <Basil/Window.hpp>

using basil::BasilApp;
using basil::FileTextureSource;
using basil::GLVertexShader;
using basil::GLFragmentShader;
using basil::GLShaderProgram;
using basil::GLShaderPane;
using basil::GLTexture2D;
using basil::LogLevel;
using basil::MetricsReporter;
using basil::PaneOrientation;
using basil::ProcessController;
using basil::ProcessPrivilege;
using basil::SpanTextureSource;
using basil::SplitPane;
using basil::WindowView;

/**
 * @brief Entry point function.
 *
 * @param argc Argument count
 * @param argv Argument value
 * @return Success code
 */
int main(int argc, char** argv) {
    auto texturePath =
        std::filesystem::path(SOURCE_DIR) / "../examples/assets/test-image.jpg";
    auto textureSource = std::make_shared<FileTextureSource>(texturePath);

    auto texture = std::make_shared<GLTexture2D>();
    texture->setSource(textureSource);
    texture->update();

    auto fragmentPath =
        std::filesystem::path(EXAMPLE_DIR) / "shaders/test.frag";
    auto program = GLShaderProgram::Builder()
        .withFragmentShaderFromFile(fragmentPath)
        .withDefaultVertexShader()
        .build();
    program->use();
    program->addTexture("testTexture", texture);

    // TODO(sholloway): Support for late builders,
    //      disallow empty objects which require content

    auto basilApp = BasilApp::Builder()
        .withWindow(WindowView::Builder()
            .withTitle("My window")
            .withDimensions(683, 1024)
            .withTopPane(GLShaderPane::Builder()
                .withShaderProgram(std::move(program))
                .build())
            .build())
        .withController(ProcessController::Builder()
            .withFrameCap(30)
            .withLateProcess(MetricsReporter::Builder()
                .withLogLevel(LogLevel::INFO)
                .withRegularity(30)
                .build())
            .build())
        .build();

    basilApp->run();

    return 0;
}
