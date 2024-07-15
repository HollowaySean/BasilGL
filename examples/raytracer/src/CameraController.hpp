#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <Basil/App.hpp>

#include "Camera.hpp"

namespace basil::raytracer {

class CameraController : public IBasilWidget,
                         public IBuildable<CameraController> {
 public:
    CameraController();

    void onStart() override;

    void onLoop() override;

    void onStop() override;

    void setFocusPane(std::shared_ptr<IPane> focusPane) {
        this->focusPane = focusPane;
    }

    void setPosition(glm::vec3 setPosition);

    void setPosition(float x, float y, float z) {
        setPosition(glm::vec3(x, y, z));
    }

    void setTilt(float setTiltAngle);

    void setMaximumTiltAngle(float setMaxTiltAngle);

    class Builder : public IBuilder<CameraController> {
     public:
        Builder& withFocusPane(std::shared_ptr<IPane> focusPane) {
            this->impl->setFocusPane(focusPane);
            return (*this);
        }

        Builder& withPosition(float x, float y, float z) {
            this->impl->setPosition(x, y, z);
            return (*this);
        }

        Builder& withPosition(glm::vec3 setPosition) {
            this->impl->setPosition(setPosition);
            return (*this);
        }

        Builder& withTiltAngle(float tiltAngle) {
            this->impl->setTilt(tiltAngle);
            return (*this);
        }

        Builder& withMaximumTilt(float maxTiltAngle) {
            this->impl->setMaximumTiltAngle(maxTiltAngle);
            return (*this);
        }
    };

 private:
    Camera camera = Camera();
    std::shared_ptr<IPane> focusPane = nullptr;

    float getDeltaTime();
    void onResize(int width, int height);

    void checkControlLock();
    bool mousePositionCurrent = false;
    bool controlsActive = false;

    void updatePosition(float deltaTime);
    void updateRotation(float deltaTime);
    void updateProjectionUniforms();

    UserInputWatcher userInputWatcher;
    UserInputModel& inputModel = userInputWatcher.getModel();

    float moveSpeed = 10.0f;
    float mouseSpeed = 10.0f;

    UserInputModel::MousePosition lastMousePosition;

    FrameClock::time_point lastFrameTime;

    ShaderUniformModel uniformModel;

    float tiltAngle = 0.0f;
    float maxTiltAngle = 80.0f;

    glm::vec3 position = glm::vec3();
    glm::mat4 inverseView = glm::mat4();
    glm::mat4 inverseProjection = glm::mat4();

    unsigned int invViewID    = -1;
    unsigned int invProjID    = -1;
    unsigned int positionID   = -1;
    unsigned int resolutionID = -1;

    unsigned int callbackID = -1;
};

}  // namespace basil::raytracer
