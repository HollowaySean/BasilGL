#include "ShadertoyUniformPublisher.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace basil {

ShadertoyUniformPublisher::ShadertoyUniformPublisher() : IBasilWidget({
        "ShadertoyUniformPublisher",
        ProcessOrdinal::EARLY,
        ProcessPrivilege::NONE,
        WidgetPubSubPrefs::PUBLISH_ONLY
    }) {}

void ShadertoyUniformPublisher::onStart() {
    timeWatcher.onStart();
    inputWatcher.onStart();

    initializeUniforms();
}

void ShadertoyUniformPublisher::initializeUniforms() {
    resolutionID =  uniformModel.addUniformValue(
        std::vector<float>({0., 0., 0.}), RESOLUTION_UNIFORM_NAME);
    mouseID =       uniformModel.addUniformValue(
        std::vector<float>({0., 0., 0., 0.}), MOUSE_UNIFORM_NAME);
    timeID =        uniformModel.addUniformValue(
        0.f, TIME_UNIFORM_NAME);
    deltaTimeID =   uniformModel.addUniformValue(
        0.f, DELTATIME_UNIFORM_NAME);
    frameRateID =   uniformModel.addUniformValue(
        0, FRAMERATE_UNIFORM_NAME);
}

void ShadertoyUniformPublisher::onLoop() {
    timeWatcher.onLoop();
    inputWatcher.onLoop();

    setResolution();
    setMouse();
    setTime();

    this->IDataPublisher::publishData(DataMessage(uniformModel));
}

void ShadertoyUniformPublisher::setResolution() {
    ViewArea viewArea;
    if (focusPane) {
        viewArea = focusPane->viewArea;
    } else {
        auto windowSize = inputModel.getWindowSize();
        viewArea = ViewArea { windowSize.width, windowSize.height, 0, 0 };
    }

    resolution_x = static_cast<float>(viewArea.width);
    resolution_y = static_cast<float>(viewArea.height);

    std::vector<float> iResolution =
        { resolution_x, resolution_y, PIXEL_ASPECT_RATIO };
    uniformModel.setUniformValue(iResolution, resolutionID);
}

void ShadertoyUniformPublisher::setMouse() {
    auto mouseState = inputModel.getMousePosition();

    // Cast mouse coordinates to float and correct y-axis
    float raw_x = static_cast<float>(mouseState.xPosition);
    float raw_y;
    if (INVERT_Y_AXIS) {
        raw_y = resolution_y - static_cast<float>(mouseState.yPosition);
    } else {
        raw_y = static_cast<float>(mouseState.yPosition);
    }

    bool isClicking =
        inputModel.getIsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
    bool isClickStart = isClicking && !wasClicking;

    // iMouse.xy contains coordinates of last clicked position
    if (isClicking) {
        lastDown_x = raw_x;
        lastDown_y = raw_y;
    }

    // iMouse.zw contains coordinates of last start of click
    if (isClickStart) {
        lastStart_x = raw_x;
        lastStart_y = raw_y;
    }
    wasClicking = isClicking;

    // iMouse.zw are signed based on current click logic
    std::vector<float> iMouse =
        {   lastDown_x,
            lastDown_y,
            (isClicking   ? 1 : -1) * lastStart_x,
            (isClickStart ? 1 : -1) * lastStart_y
        };
    uniformModel.setUniformValue(iMouse, mouseID);
}

void ShadertoyUniformPublisher::setTime() {
    float iTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        timeModel.timeSinceStart).count() / 1'000'000'000.;
    uniformModel.setUniformValue(iTime, timeID);

    float iDeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(
        timeModel.timeSinceFrame).count() / 1'000'000'000.;
    uniformModel.setUniformValue(iDeltaTime, deltaTimeID);

    float iFrameRate = timeModel.frameRate();
    uniformModel.setUniformValue(iFrameRate, frameRateID);
}

ShadertoyUniformPublisher::Builder&
ShadertoyUniformPublisher::Builder::withFocusPane(
        std::shared_ptr<IPane> focusPane) {
    this->impl->setFocusPane(focusPane);
    return (*this);
}


}  // namespace basil
