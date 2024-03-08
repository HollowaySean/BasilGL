#include "BasilApp.hpp"

namespace basil {

// TODO(sholloway): Log messages
void BasilApp::run() {
    if (!processController) return;

    processController->run();
}


void BasilApp::stop() {
    if (!processController) return;

    processController->stop();
}


void BasilApp::kill() {
    if (!processController) return;

    processController->kill();
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
