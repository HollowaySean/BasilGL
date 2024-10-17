#include <catch.hpp>

#include "Widget/ShadertoyUniformPublisher.hpp"

#include "PubSub/PubSubTestUtils.hpp"
#include "Window/WindowTestUtils.hpp"

using basil::ShadertoyUniformPublisher;
using basil::TestSubscriber;
using basil::UserInputWatcher;

TEST_CASE("Widget_ShadertoyUniformPublisher_onStart") {
    auto widget = ShadertoyUniformPublisher();
    widget.onStart();

    SECTION("Initializes uniforms in model") {
        auto model = widget.uniformModel;
        auto uniform = model.getUniform(widget.RESOLUTION_UNIFORM_NAME);

        CHECK(uniform.has_value());
    }
}

TEST_CASE("Widget_ShadertoyUniformPublisher_onLoop") {
    auto widget = ShadertoyUniformPublisher();

    auto subscriber = std::make_shared<TestSubscriber>();
    widget.subscribe(subscriber);


    SECTION("Sets uniforms") {
        widget.INVERT_Y_AXIS = false;

        widget.onStart();
        widget.onLoop();

        auto& model = widget.getModel();
        auto iMouse = model.getUniform(
            ShadertoyUniformPublisher::MOUSE_UNIFORM_NAME);

        REQUIRE(iMouse.has_value());
        CHECK((reinterpret_cast<float*>(iMouse.value()->getData()))[0]
            == UserInputWatcher::TEST_MOUSE_X_POSITION);
        CHECK((reinterpret_cast<float*>(iMouse.value()->getData()))[1]
            == UserInputWatcher::TEST_MOUSE_Y_POSITION);
        CHECK((reinterpret_cast<float*>(iMouse.value()->getData()))[2]
            == UserInputWatcher::TEST_MOUSE_X_POSITION);
        CHECK((reinterpret_cast<float*>(iMouse.value()->getData()))[3]
            == UserInputWatcher::TEST_MOUSE_Y_POSITION);
    }

    SECTION("Publishes data") {
        CHECK_FALSE(subscriber->hasReceivedData);
        widget.onLoop();
        CHECK(subscriber->hasReceivedData);
    }
}

TEST_CASE("Widget_ShadertoyUniformPublisher_setFocusPane") {
    auto widget = ShadertoyUniformPublisher();
    auto pane = std::make_shared<TestPane>(testViewArea);

    SECTION("Uses focus pane for resolution") {
        widget.setFocusPane(pane);
        widget.onStart();
        widget.onLoop();

        auto uniformOpt = widget.uniformModel.getUniform("iResolution");
        REQUIRE(uniformOpt.has_value());

        CHECK((reinterpret_cast<float*>(uniformOpt.value()->getData()))[0]
            == testViewArea.width);
        CHECK((reinterpret_cast<float*>(uniformOpt.value()->getData()))[1]
            == testViewArea.height);
        CHECK((reinterpret_cast<float*>(uniformOpt.value()->getData()))[2]
            == BASIL_PIXEL_ASPECT_RATIO);
    }
}

TEST_CASE("Widget_ShadertoyUniformPublisher_Builder") {
    SECTION("Builds correctly") {
        auto focus = std::make_shared<TestPane>(testViewArea);
        auto widget = ShadertoyUniformPublisher::Builder()
            .withFocusPane(focus)
            .build();

        CHECK(widget->focusPane == focus);
    }
}
