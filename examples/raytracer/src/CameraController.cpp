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
    aspectRatio = static_cast<float>(width - sidebarWidth) / height;

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
    aspectRatio = static_cast<float>(width - sidebarWidth) / height;
}

void CameraController::updatePosition(float deltaTime) {
    glm::vec3 moveDirection = glm::vec3(0.0f);

    if (inputModel.getIsKeyPressed(GLFW_KEY_W)) {
        moveDirection += direction;
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_S)) {
        moveDirection -= direction;
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_A)) {
        moveDirection -= glm::normalize(
            glm::cross(worldUp, direction));
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_D)) {
        moveDirection += glm::normalize(
            glm::cross(worldUp, direction));
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_E)) {
        moveDirection += worldUp;
    }
    if (inputModel.getIsKeyPressed(GLFW_KEY_Q)) {
        moveDirection -= worldUp;
    }

    if (glm::length(moveDirection) > 0) {
        moveDirection = glm::normalize(moveDirection);
    }

    position += moveDirection * moveSpeed * deltaTime;
}

void CameraController::updateRotation(float deltaTime) {
    UserInputModel::MousePosition currentMouse
        = inputModel.getMousePosition();

    double deltaX = currentMouse.xPosition - lastMousePosition.xPosition;
    double deltaY = currentMouse.yPosition - lastMousePosition.yPosition;
    lastMousePosition = currentMouse;

    direction = glm::rotate(
        direction,
        static_cast<float>(glm::radians(deltaX * mouseSpeed * deltaTime)),
        worldUp);
    direction = glm::rotate(
        direction,
        static_cast<float>(glm::radians(deltaY * mouseSpeed * deltaTime)),
        cameraRight);
}

void CameraController::updateProjectionUniforms() {
    cameraRight = glm::normalize(
        glm::cross(worldUp, direction));
    cameraUp =
        glm::cross(direction, cameraRight);

    glm::mat4 view = glm::lookAtLH(
        position,
        position + direction,
        cameraUp);
    inverseView = glm::inverse(view);

    glm::mat4 project = glm::perspectiveLH(
        glm::radians(fov), aspectRatio, 0.1f, 1000.0f);
    inverseProjection = glm::inverse(project);

    uniformModel.setUniformValue(
        glm::value_ptr(inverseView), invViewID);
    uniformModel.setUniformValue(
        glm::value_ptr(inverseProjection), invProjID);
    uniformModel.setUniformValue(
        glm::value_ptr(position), positionID);
}

}  // namespace basil::raytracer
