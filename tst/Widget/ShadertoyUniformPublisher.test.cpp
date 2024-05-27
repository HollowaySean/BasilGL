#include <catch.hpp>

#include "Widget/ShadertoyUniformPublisher.hpp"

#include "PubSub/PubSubTestUtils.hpp"
#include "Window/WindowTestUtils.hpp"

using basil::GLUniformDirectVector;
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

        CHECK(iMouse.has_value());
        CHECK(iMouse.value().value == GLUniformDirectVector(std::vector<float>({
            UserInputWatcher::TEST_MOUSE_X_POSITION,
            UserInputWatcher::TEST_MOUSE_Y_POSITION,
            UserInputWatcher::TEST_MOUSE_X_POSITION,
            UserInputWatcher::TEST_MOUSE_Y_POSITION
        })));
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

        GLUniformDirectVector iResolution = uniformOpt.value().value;
        CHECK(iResolution == GLUniformDirectVector(std::vector<float>({
            static_cast<float>(testViewArea.width),
            static_cast<float>(testViewArea.height),
            BASIL_PIXEL_ASPECT_RATIO
        })));
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
