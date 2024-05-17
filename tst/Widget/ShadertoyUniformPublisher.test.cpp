#include <catch.hpp>

#include "Widget/ShadertoyUniformPublisher.hpp"

#include "PubSub/PubSubTestUtils.hpp"

using basil::GLUniformType;
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
        CHECK(iMouse.value().value == GLUniformType(std::vector<float>({
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
