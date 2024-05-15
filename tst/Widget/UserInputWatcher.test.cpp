#include <catch.hpp>

#include "Widget/UserInputWatcher.hpp"

#include "PubSub/PubSubTestUtils.hpp"
#include "Window/GLTestUtils.hpp"

using basil::UserInputWatcher;
using basil::TestSubscriber;

TEST_CASE("Widget_UserInputWatcher_onStart") { BASIL_LOCK_TEST
    auto widget = UserInputWatcher();
    auto& model = widget.getModel();

    widget.onStart();

    SECTION("Initializes model states") {
        CHECK(model.getKeyState(GLFW_KEY_A).pressState == GLFW_PRESS);
        CHECK(model.getMousePosition().xPosition ==
            UserInputWatcher::TEST_MOUSE_X_POSITION);
        CHECK(model.getMousePosition().yPosition ==
            UserInputWatcher::TEST_MOUSE_Y_POSITION);
        CHECK(widget.model.getMousePosition().isInWindow);
    }
}

TEST_CASE("Widget_UserInputWatcher_onLoop") { BASIL_LOCK_TEST
    auto widget = UserInputWatcher();

    SECTION("Updates mouse state") {
        widget.model.mousePosition.xPosition = 0;
        widget.model.mousePosition.yPosition = 0;
        widget.model.mousePosition.isInWindow = false;

        widget.onLoop();

        CHECK(widget.model.getMousePosition().xPosition ==
            UserInputWatcher::TEST_MOUSE_X_POSITION);
        CHECK(widget.model.getMousePosition().yPosition ==
            UserInputWatcher::TEST_MOUSE_Y_POSITION);
        CHECK_FALSE(widget.model.getMousePosition().isInWindow);
    }

    SECTION("Sends data to PubSub subscriber") {
        auto subscriber = std::make_shared<TestSubscriber>();
        widget.subscribe(subscriber);

        CHECK_FALSE(subscriber->hasReceivedData);
        widget.onLoop();
        CHECK(subscriber->hasReceivedData);
    }
}
