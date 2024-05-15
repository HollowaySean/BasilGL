#include <catch.hpp>

#include <Basil/App.hpp>

#include "AppTestUtils.hpp"

using basil::BasilApp;
using basil::IBasilWidget;
using basil::Logger;
using basil::LogLevel;
using basil::ProcessController;
using basil::ProcessControllerState;
using basil::TestWidget;
using basil::WindowView;

TEST_CASE("App_BasilApp_run") {
    auto controller = std::make_shared<ProcessController>();
    auto app = BasilApp();

    SECTION("Calls run on controller") {
        app.setController(controller);

        CHECK(controller->getCurrentState() ==
            ProcessControllerState::READY);
        app.run();
        CHECK(controller->getCurrentState() ==
            ProcessControllerState::STOPPED);
    }

    SECTION("Logs error if controller is missing") {
        Logger& logger = Logger::get();
        app.run();

        CHECK(logger.getLastOutput()
            == "ProcessController not found for BasilApp.");
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }
}

TEST_CASE("App_BasilApp_stop") {
    auto controller = std::make_shared<ProcessController>();
    auto app = BasilApp();

    SECTION("Calls stop on controller") {
        app.setController(controller);

        CHECK(controller->getCurrentState() ==
            ProcessControllerState::READY);
        app.stop();
        CHECK(controller->getCurrentState() ==
            ProcessControllerState::STOPPING);
    }

    SECTION("Logs error if controller is missing") {
        Logger& logger = Logger::get();
        app.stop();

        CHECK(logger.getLastOutput()
            == "ProcessController not found for BasilApp.");
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }
}

TEST_CASE("App_BasilApp_kill") {
    auto controller = std::make_shared<ProcessController>();
    auto app = BasilApp();

    SECTION("Calls kill on controller") {
        app.setController(controller);

        CHECK(controller->getCurrentState() ==
            ProcessControllerState::READY);
        app.kill();
        CHECK(controller->getCurrentState() ==
            ProcessControllerState::KILLED);
    }

    SECTION("Logs error if controller is missing") {
        Logger& logger = Logger::get();
        app.kill();

        CHECK(logger.getLastOutput()
            == "ProcessController not found for BasilApp.");
        CHECK(logger.getLastLevel() == LogLevel::WARN);
    }
}

TEST_CASE("App_BasilApp_autoWire") {
    auto controller = std::make_shared<ProcessController>();
    auto window = std::make_shared<WindowView>();
    std::shared_ptr<IBasilWidget> widget
        = std::make_shared<TestWidget>();
    auto basilApp = BasilApp();

    basilApp.addWidget(widget);
    basilApp.setWindow(window);
    basilApp.setController(controller);

    basilApp.autoWire();

    SECTION("Adds processes to controller") {
        CHECK(controller->hasProcess(window));
        CHECK(controller->hasProcess(widget));
    }

    SECTION("Subscribes window to app publisher") {
        CHECK(basilApp.publisher->hasSubscriber(window));
    }
}

TEST_CASE("App_BasilApp_Builder") {
    auto controller = std::make_shared<ProcessController>();
    auto window = std::make_shared<WindowView>();
    std::shared_ptr<IBasilWidget> widget
        = std::make_shared<TestWidget>();

    auto basilApp = BasilApp::Builder()
        .withWidget(widget)
        .withWindow(window)
        .withController(controller)
        .build();

    SECTION("Builds from components") {
        CHECK(basilApp->processController == controller);
        CHECK(basilApp->windowView == window);
        CHECK(basilApp->widgets.front() == widget);
    }

    SECTION("Adds processes to controller") {
        CHECK(controller->hasProcess(window));
        CHECK(controller->hasProcess(widget));
    }

    SECTION("Subscribes app to widget") {
        CHECK(widget->hasSubscriber(basilApp->publisher));
    }
}
