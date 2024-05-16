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
using basil::WidgetPubSubPrefs;

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
    std::shared_ptr<IBasilWidget> widget1
        = std::make_shared<TestWidget>();
    widget1->pubSubPrefs = WidgetPubSubPrefs::PUBLISH_ONLY;

    std::shared_ptr<IBasilWidget> widget2
        = std::make_shared<TestWidget>();
    widget2->pubSubPrefs = WidgetPubSubPrefs::SUBSCRIBE_ONLY;

    std::shared_ptr<IBasilWidget> widget3
        = std::make_shared<TestWidget>();
    widget3->pubSubPrefs = WidgetPubSubPrefs::ALL;

    auto basilApp = BasilApp();

    basilApp.addWidget(widget1);
    basilApp.addWidget(widget2);
    basilApp.addWidget(widget3);
    basilApp.setController(controller);

    basilApp.autoWire();

    SECTION("Adds processes to controller") {
        CHECK(controller->hasProcess(widget1));
        CHECK(controller->hasProcess(widget2));
        CHECK(controller->hasProcess(widget3));
    }

    SECTION("Subscribes based on prefs") {
        CHECK_FALSE(basilApp.publisher->hasSubscriber(widget1));
        CHECK(basilApp.publisher->hasSubscriber(widget2));
        CHECK(basilApp.publisher->hasSubscriber(widget3));

        CHECK(widget1->hasSubscriber(basilApp.publisher));
        CHECK_FALSE(widget2->hasSubscriber(basilApp.publisher));
        CHECK(widget3->hasSubscriber(basilApp.publisher));
    }
}

TEST_CASE("App_BasilApp_Builder") {
    auto controller = std::make_shared<ProcessController>();
    std::shared_ptr<IBasilWidget> widget
        = std::make_shared<TestWidget>();

    auto basilApp = BasilApp::Builder()
        .withWidget(widget)
        .withController(controller)
        .build();

    SECTION("Builds from components") {
        CHECK(basilApp->processController == controller);
        CHECK(basilApp->widgets.front() == widget);
    }

    SECTION("Adds processes to controller") {
        CHECK(controller->hasProcess(widget));
    }

    SECTION("Subscribes app to widget") {
        CHECK(widget->hasSubscriber(basilApp->publisher));
        CHECK(basilApp->publisher->hasSubscriber(widget));
    }
}
