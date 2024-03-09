#include <catch.hpp>

#include <Basil/App.hpp>

using basil::BasilApp;
using basil::ProcessController;
using basil::ProcessControllerState;
using basil::WindowView;

TEST_CASE("App_BasilApp_run") {
    auto controller = std::make_shared<ProcessController>();
    auto app = BasilApp();
    app.setController(controller);

    SECTION("Calls run on controller") {
        REQUIRE(controller->getCurrentState() ==
            ProcessControllerState::READY);
        app.run();
        REQUIRE(controller->getCurrentState() ==
            ProcessControllerState::STOPPED);
    }
}

TEST_CASE("App_BasilApp_stop") {
    auto controller = std::make_shared<ProcessController>();
    auto app = BasilApp();
    app.setController(controller);

    SECTION("Calls stop on controller") {
        REQUIRE(controller->getCurrentState() ==
            ProcessControllerState::READY);
        app.stop();
        REQUIRE(controller->getCurrentState() ==
            ProcessControllerState::STOPPING);
    }
}

TEST_CASE("App_BasilApp_kill") {
    auto controller = std::make_shared<ProcessController>();
    auto app = BasilApp();
    app.setController(controller);

    SECTION("Calls kill on controller") {
        REQUIRE(controller->getCurrentState() ==
            ProcessControllerState::READY);
        app.kill();
        REQUIRE(controller->getCurrentState() ==
            ProcessControllerState::KILLED);
    }
}

TEST_CASE("App_BasilApp_Builder") {
    auto controller = std::make_shared<ProcessController>();
    auto window = std::make_shared<WindowView>();

    auto basilApp = BasilApp::Builder()
        .withWindow(window)
        .withController(controller)
        .build();

    SECTION("Builds from components") {
        REQUIRE(basilApp->processController == controller);
        REQUIRE(basilApp->windowView == window);
    }

    SECTION("Adds window process to controller") {
        REQUIRE(controller->schedule.back()->process
            == window);
    }
}
