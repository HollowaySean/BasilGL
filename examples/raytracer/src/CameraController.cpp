#include "CameraController.hpp"

namespace basil::raytracer {

CameraController::CameraController() : IBasilWidget({
        "CameraController",
        ProcessOrdinal::EARLY,
        ProcessPrivilege::NONE,
        WidgetPubSubPrefs::PUBLISH_ONLY
    }) {}

void CameraController::onStart() {
    lastFrameTime = FrameClock::now();

    invViewID = uniformModel.addUniform(
        std::make_shared<GLUniformPointer<float>>(
            glm::value_ptr(inverseView),
                "inverseView", 4, 4, 1));

    invProjID = uniformModel.addUniform(
        std::make_shared<GLUniformPointer<float>>(
            glm::value_ptr(inverseProjection),
                "inverseProjection", 4, 4, 1));

    positionID = uniformModel.addUniform(
        std::make_shared<GLUniformPointer<float>>(
            glm::value_ptr(position),
                "position", 3, 1, 1));

    userInputWatcher.onStart();

    lastMousePosition = inputModel.getMousePosition();

    // Should probably remove this on destructor
    using std::placeholders::_1, std::placeholders::_2;
    callbackID = BasilContext::setGLFWFramebufferSizeCallback(
        std::bind(&CameraController::onResize, this, _1, _2));

    // Set initial aspect ratio
    auto window = BasilContext::getGLFWWindow();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    onResize(width, height);

    // Set initial camera position
    camera.setPosition(glm::vec3(0, 1, -1));
    camera.setOrientation(
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1));
    camera.tilt(-30.0f);

    publishData(DataMessage(uniformModel));
}

void CameraController::onLoop() {
    float deltaTime = getDeltaTime();
    userInputWatcher.onLoop();

    updatePosition(deltaTime);
    updateRotation(deltaTime);
    updateProjectionUniforms();

    publishData(DataMessage(uniformModel));
}

float CameraController::getDeltaTime() {
    auto currentTime = FrameClock::now();
    auto elapsedTime = currentTime - lastFrameTime;
    lastFrameTime = currentTime;

    float elapsedTimeNS = std::chrono::duration_cast<
        std::chrono::nanoseconds>(elapsedTime).count();
    float elapsedTimeSeconds = elapsedTimeNS / 1'000'000'000;

    return elapsedTimeSeconds;
}

void CameraController::onResize(int width, int height) {
    if (focusPane) {
        width = focusPane->viewArea.width;
        height = focusPane->viewArea.height;
    }

    float aspectRatio = static_cast<float>(width) / height;
    camera.setAspectRatio(aspectRatio);
}

void CameraController::updatePosition(float deltaTime) {
    glm::vec3 moveDirection = glm::vec3(0);

    if (inputModel.getIsKeyPressed(GLFW_KEY_W)) {
        moveDirection += glm::vec3(0, 0, 1);
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_S)) {
        moveDirection -= glm::vec3(0, 0, 1);
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_A)) {
        moveDirection -= glm::vec3(1, 0, 0);
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_D)) {
        moveDirection += glm::vec3(1, 0, 0);
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_E)) {
        moveDirection += glm::vec3(0, 1, 0);
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_Q)) {
        moveDirection -= glm::vec3(0, 1, 0);
    }

    if (glm::length(moveDirection) > 0) {
        moveDirection = glm::normalize(moveDirection);
    }

    camera.moveRelative(moveDirection * moveSpeed * deltaTime);
}

void CameraController::updateRotation(float deltaTime) {
    UserInputModel::MousePosition currentMouse
        = inputModel.getMousePosition();

    double deltaX = currentMouse.xPosition - lastMousePosition.xPosition;
    double deltaY = currentMouse.yPosition - lastMousePosition.yPosition;
    deltaY *= -1;  // Reverse value so positive is up
    lastMousePosition = currentMouse;

    camera.pan(deltaX * mouseSpeed * deltaTime);
    camera.tilt(deltaY * mouseSpeed * deltaTime);
}

void CameraController::updateProjectionUniforms() {
    inverseView = camera.getInverseViewMatrix();
    uniformModel.setUniformValue(
        glm::value_ptr(inverseView), invViewID);

    inverseProjection = camera.getInverseProjectionMatrix();
    uniformModel.setUniformValue(
        glm::value_ptr(inverseProjection), invProjID);

    position = camera.getPosition();
    uniformModel.setUniformValue(
        glm::value_ptr(position), positionID);
}

}  // namespace basil::raytracer
