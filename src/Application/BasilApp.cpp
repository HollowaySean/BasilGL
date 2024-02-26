#include "BasilApp.hpp"

namespace basil {

BasilApp::BasilApp() {
    this->windowView =
        std::make_shared<WindowView>();
    this->frameController =
        std::make_shared<FrameController>();

    frameController->addProcess(windowView.get(),
        FrameController::Privilege::HIGH,
        "Main window render");
}

BasilApp::BasilApp(Builder builder) : BasilApp() {
    if (builder.frameRateCap.has_value()) {
        this->frameController->setFrameCap(builder.frameRateCap.value());
    }

    if (builder.topPane) {
        this->topPane = builder.topPane;
        this->topPane->paneProps = windowView->getTopPaneProps();

        this->windowView->setTopPane(builder.topPane.get());
    }

    if (builder.shouldIncludeMetrics) {
        FrameMetrics *frameMetrics = &frameController->metrics;
        MetricsReporter reporter = MetricsReporter(
            frameMetrics, frameMetrics->getBufferSize());
        this->frameController->addProcess(&reporter,
            FrameController::Privilege::NONE,
            "Metrics reporter");
    }
}

void BasilApp::run() {
    frameController->run();
}

BasilApp BasilApp::Builder::build() {
    return BasilApp(*this);
}

BasilApp::Builder& BasilApp::Builder::withShaderPane(std::filesystem::path shaderPath) {
    this->topPane =
        std::make_shared<GLTexturePane>(PaneProps(), shaderPath);
    return *this;
}

BasilApp::Builder& BasilApp::Builder::withMetricsReporter(int bufferSize) {
    this->shouldIncludeMetrics = true;
    this->bufferSize = bufferSize;
    return *this;
}

BasilApp::Builder& BasilApp::Builder::withFrameRateCap(int maxFrameRate) {
    this->frameRateCap = maxFrameRate;
    return *this;
}

}  // namespace basil