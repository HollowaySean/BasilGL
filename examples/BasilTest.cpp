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

    std::vector<float> values(500*500*4, 1.);
    auto spanSource = std::make_shared<SpanTextureSource<float, 2, 4>>(values);
    spanSource->setWidth(500);
    spanSource->setHeight(500);

    auto spanTexture = std::make_shared<GLTexture2D>();
    spanTexture->setSource(spanSource);
    spanTexture->update();

    auto fragmentPath =
        std::filesystem::path(EXAMPLE_DIR) / "shaders/test.frag";
    auto program = GLShaderProgram::Builder()
        .withFragmentShaderFromFile(fragmentPath)
        .withDefaultVertexShader()
        .build();
    program->use();
    program->addTexture("testTexture", texture);

    auto basilApp = BasilApp::Builder()
        .withWindow(WindowView::Builder()
            .withTitle("My window")
            .withDimensions(500, 500)
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
