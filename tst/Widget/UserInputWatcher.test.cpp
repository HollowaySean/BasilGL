#include <catch.hpp>

#include "Widget/UserInputWatcher.hpp"

#include "PubSub/PubSubTestUtils.hpp"
#include "Window/GLTestUtils.hpp"

using basil::UserInputWatcher;
using basil::TestSubscriber;

TEST_CASE("Widget_UserInputWatcher_onStart") {
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

TEST_CASE("Widget_UserInputWatcher_onLoop") {
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

TEST_CASE("Widget_UserInputWatcher_onMouseButtonChange") {
    auto widget = UserInputWatcher();
    auto& model = widget.getModel();

    SECTION("Sets mouse button state") {
        widget.onMouseButtonChange(
            GLFW_MOUSE_BUTTON_1, GLFW_PRESS, GLFW_MOD_ALT);

        auto state = model.getMouseButtonState(GLFW_MOUSE_BUTTON_1);
        CHECK(state.pressState == GLFW_PRESS);
        CHECK(state.modifiers == GLFW_MOD_ALT);
    }
}

TEST_CASE("Widget_UserInputWatcher_onKeyChange") {
    auto widget = UserInputWatcher();
    auto& model = widget.getModel();

    SECTION("Sets key state") {
        widget.onKeyChange(
            GLFW_KEY_0, 0, GLFW_RELEASE, GLFW_MOD_CAPS_LOCK);

        auto state = model.getKeyState(GLFW_KEY_0);
        CHECK(state.pressState == GLFW_RELEASE);
        CHECK(state.modifiers == GLFW_MOD_CAPS_LOCK);
    }
}


TEST_CASE("Widget_UserInputWatcher_onCursorEnter") {
    auto widget = UserInputWatcher();
    auto& model = widget.getModel();

    SECTION("Sets boolean indicating cursor state") {
        widget.onCursorEnter(GLFW_TRUE);

        CHECK(model.getIsMouseInWindow());
    }
}
