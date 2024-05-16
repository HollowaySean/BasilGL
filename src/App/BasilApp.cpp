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
    for (auto widget : widgets) {
        autoWireWidget(widget);
    }
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
    bool shouldSubscribe =
        widget
        && !publisher->hasSubscriber(widget)
        && widget->shouldSubscribeToApp();

    if (shouldSubscribe) {
        publisher->subscribe(widget);
    }

    bool shouldPublish =
        widget
        && !widget->hasSubscriber(publisher)
        && widget->shouldPublishToApp();

    if (shouldPublish) {
        widget->subscribe(publisher);
    }
}

void BasilApp::autoWireWidget(
        std::shared_ptr<IBasilWidget> widget) {
    autoWireWidgetPublisher(widget);

    if (processController) {
        autoWireWidgetProcess(widget);
    }
}

BasilApp::Builder&
BasilApp::Builder::withController(
        std::shared_ptr<ProcessController> controller) {
    impl->setController(controller);
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
