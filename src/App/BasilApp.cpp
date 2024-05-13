#include "BasilApp.hpp"

namespace basil {

void BasilApp::run() {
    autoWire();

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

void BasilApp::addWidget(std::shared_ptr<IBasilWidget> widget) {
    widgets.emplace_back(widget);
}

void BasilApp::logControllerMissing() {
    logger.log("ProcessController not found for BasilApp.",
        LogLevel::WARN);
}

void BasilApp::autoWire() {
    autoWireWindowProcess();

    for (auto widget : widgets) {
        autoWireWidget(widget);
    }
}

void BasilApp::autoWireWindowProcess() {
    if (!(this->processController && this->windowView)) return;

    if (!(publisher->hasSubscriber(windowView))) {
        publisher->subscribe(windowView);
    }

    if (this->processController->hasProcess(windowView)) return;

    this->processController->addProcess(
        windowView, ProcessPrivilege::HIGH);
}

void BasilApp::autoWireWidgetProcess(
        std::shared_ptr<IBasilWidget> widget) {
    if (widget && !processController->hasProcess(widget)) {
        processController->addProcessWithOrdinal(
            widget, widget->ordinal, widget->privilege);
    }
}

void BasilApp::autoWireWidgetPublisher(
        std::shared_ptr<IBasilWidget> widget) {
    if (widget && !publisher->hasSubscriber(widget)) {
        publisher->subscribe(widget);
    }

    if (widget && !widget->hasSubscriber(publisher)) {
        widget->subscribe(publisher);
    }
}

void BasilApp::autoWireWidget(
        std::shared_ptr<IBasilWidget> widget) {
    autoWireWidgetPublisher(widget);

    if (processController) {
        autoWireWidgetProcess(widget);
    }

    if (windowView) {
        autoWireWidgetPublisher(widget);
    }
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

BasilApp::Builder&
BasilApp::Builder::withWidget(std::shared_ptr<IBasilWidget> widget) {
    impl->addWidget(widget);
    return *this;
}

std::shared_ptr<BasilApp>
BasilApp::Builder::build() {
    impl->autoWire();

    return IBuilder<BasilApp>::build();
}

}  // namespace basil
