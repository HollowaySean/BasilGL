#include <Basil/App.hpp>

#include "ImGuiExamplePane.hpp"

static inline const std::string RED_SHADER =
    "#version 450 core\nout vec4 FragColor;\n"
    "void main() { FragColor = vec4(1.0, 0.0, 0.0, 1.0); }\0";
static inline const std::string YELLOW_SHADER =
    "#version 450 core\nout vec4 FragColor;\n"
    "void main() { FragColor = vec4(1.0, 1.0, 0.0, 1.0); }\0";
static inline const std::string GREEN_SHADER =
    "#version 450 core\nout vec4 FragColor;\n"
    "void main() { FragColor = vec4(0.0, 1.0, 0.0, 1.0); }\0";
static inline const std::string BLUE_SHADER =
    "#version 450 core\nout vec4 FragColor;\n"
    "void main() { FragColor = vec4(0.0, 0.0, 1.0, 1.0); }\0";

using basil::BasilApp;
using basil::GLShaderPane;
using basil::ProcessController;
using basil::SplitPane;
using basil::WindowView;

/**
 * @brief BasilGL example project / functionality test which demonstrates
 * a how a complicated nesting of windows can be set with an (admittedly
 * also complicated) nesting of builders. See more information in sidebar
 * upon building and running the executable.
 */
int main() {
    auto basilApp = BasilApp::Builder()
        .withController(ProcessController::Builder()
            .withFrameCap(60)
            .build())
        .withWidget(WindowView::Builder()
            .withTitle("BasilGL Panes Demo")
            .withDimensions(1000, 500)
            .withTopPane(SplitPane::Builder()
                .withFixedPane(SplitPane::FixedPane::SECOND)
                .withOrientation(SplitPane::Orientation::HORIZONTAL)
                .withPaneExtentInPixels(300)
                .withFirstPane(SplitPane::Builder()
                    .withFixedPane(SplitPane::FixedPane::FIRST)
                    .withOrientation(SplitPane::Orientation::HORIZONTAL)
                    .withPaneAspectRatio(1.0)
                    .withGapWidth(4)
                    .withFirstPane(GLShaderPane::Builder()
                        .fromShaderCode(RED_SHADER)
                        .build())
                    .withSecondPane(SplitPane::Builder()
                        .withFixedPane(SplitPane::FixedPane::FIRST)
                        .withOrientation(SplitPane::Orientation::VERTICAL)
                        .withPaneExtentInPercent(75.)
                        .withGapWidth(2)
                        .withFirstPane(GLShaderPane::Builder()
                            .fromShaderCode(GREEN_SHADER)
                            .build())
                        .withSecondPane(SplitPane::Builder()
                            .withFixedPane(SplitPane::FixedPane::FIRST)
                            .withOrientation(SplitPane::Orientation::HORIZONTAL)
                            .withPaneExtentInPercent(50.)
                            .withFirstPane(GLShaderPane::Builder()
                                .fromShaderCode(BLUE_SHADER)
                                .build())
                            .withSecondPane(GLShaderPane::Builder()
                                .fromShaderCode(YELLOW_SHADER)
                                .build())
                            .build())
                        .build())
                    .build())
                .withSecondPane(ImGuiExamplePane::Builder()
                    .build())
                .build())
            .build())
        .build();

    basilApp->run();

    return 0;
}
