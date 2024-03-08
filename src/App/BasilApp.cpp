#include "BasilApp.hpp"

namespace basil {

void BasilApp::run() {
    if (!processController) {
        logControllerMissing();
        return;
    }

    processController->run();
}


void BasilApp::stop() {
    if (!processController) {
        logControllerMissing();
        return;
    }

    processController->stop();
}


void BasilApp::kill() {
    if (!processController) {
        logControllerMissing();
        return;
    }

    processController->kill();
}

void BasilApp::logControllerMissing() {
    logger.log("ProcessController not found for BasilApp.",
        LogLevel::WARN);
}

BasilApp::Builder&
BasilApp::Builder::withController(
        std::shared_ptr<ProcessController> controller) {
    impl->setController(controller);
    return *this;
}

BasilApp::Builder&
BasilApp::Builder::withWindow(
        std::shared_ptr<WindowView> window) {
    impl->setWindow(window);
    return *this;
}

std::unique_ptr<BasilApp>
BasilApp::Builder::build() {
    // TODO(sholloway): Check if already added
    if (impl->processController && impl->windowView) {
        impl->processController->addProcess(
            impl->windowView, ProcessPrivilege::HIGH);
    }

    return IBuilder<BasilApp>::build();
}

}  // namespace basil
