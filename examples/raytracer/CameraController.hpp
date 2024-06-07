#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Basil/App.hpp>

using basil::DataMessage;
using basil::FrameClock;
using basil::GLUniformPointer;
using basil::IBasilWidget;
using basil::IBuildable;
using basil::ShaderUniformModel;
using basil::UserInputModel;
using basil::UserInputWatcher;

class CameraController : public IBasilWidget,
                         public IBuildable<CameraController> {
 public:
    CameraController() : IBasilWidget({
            "CameraController",
            basil::ProcessOrdinal::EARLY,
            basil::ProcessPrivilege::NONE,
            basil::WidgetPubSubPrefs::PUBLISH_ONLY
        }) {}

    void onStart() override {
        lastFrameTime = basil::FrameClock::now();

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

        publishData(DataMessage(uniformModel));
    }

    void onLoop() override {
        float deltaTime = getDeltaTime();
        userInputWatcher.onLoop();

        updatePosition(deltaTime);
        updateProjectionUniforms();

        publishData(DataMessage(uniformModel));
    }

 private:
    float getDeltaTime() {
        auto currentTime = FrameClock::now();
        auto elapsedTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        float elapsedTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
        float elapsedTimeSeconds = elapsedTimeMS / 1'000;

        return elapsedTimeSeconds;
    }

    void updatePosition(float deltaTime) {
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
        if (glm::length(moveDirection) > 0) {
            moveDirection = glm::normalize(moveDirection);
        }

        position += moveDirection * moveSpeed * deltaTime;
    }

    void updateProjectionUniforms() {
        glm::vec3 cameraRight = glm::normalize(
            glm::cross(worldUp, direction));
        glm::vec3 cameraUp =
            glm::cross(direction, cameraRight);

        glm::mat4 view = glm::lookAtLH(
            position,
            position + direction,
            cameraUp);
        inverseView = glm::inverse(view);

        glm::mat4 project = glm::perspectiveLH(
            glm::radians(fov), 1.0f, 0.1f, 1000.0f);
        inverseProjection = glm::inverse(project);

        uniformModel.setUniformValue(
            glm::value_ptr(inverseView), invViewID);
        uniformModel.setUniformValue(
            glm::value_ptr(inverseProjection), invProjID);
        uniformModel.setUniformValue(
            glm::value_ptr(position), positionID);
    }

    UserInputWatcher userInputWatcher;
    UserInputModel& inputModel = userInputWatcher.getModel();

    float fov = 60.0f;
    float moveSpeed = 10.0f;
    float mouseSpeed = 1.0f;

    FrameClock::time_point lastFrameTime;

    const glm::vec3 worldUp = glm::vec3(0, 1, 0);

    glm::vec3 position = glm::vec3(0, 1, 0);
    glm::vec3 direction = glm::vec3(0, 0, 1);

    glm::mat4 inverseView = glm::mat4(0);
    glm::mat4 inverseProjection = glm::mat4(0);

    ShaderUniformModel uniformModel;

    unsigned int invViewID  = -1;
    unsigned int invProjID  = -1;
    unsigned int positionID = -1;
};
