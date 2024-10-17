#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <Basil/App.hpp>

#include "Camera.hpp"

namespace basil::raytracer {

/** @brief Widget to read user input and update Camera object. */
class CameraController : public IBasilWidget,
                         public IBuildable<CameraController> {
 public:
    /** @brief Initialize widget. */
    CameraController();

    /** @brief Set up uniforms and register callbacks. */
    void onStart() override;

    /** @brief Read user inputs and adjust camera position. */
    void onLoop() override;

    /** @brief De-register callbacks. */
    void onStop() override;

    /** @param focusPane Pointer to pane containing camera. */
    void setFocusPane(std::shared_ptr<IPane> focusPane) {
        this->focusPane = focusPane;
    }

    /** @param setPosition New position for camera. */
    void setPosition(glm::vec3 setPosition);

    /**
     * @param x X-Value of new position.
     * @param y Y-Value of new position.
     * @param z Z-Value of new position.
     */
    void setPosition(float x, float y, float z) {
        setPosition(glm::vec3(x, y, z));
    }

    /** @param setTiltAngle Tilt angle, in degrees above horizon. */
    void setTilt(float setTiltAngle);

    /** @param setMaxTiltAngle Maximum tilt angle, in degrees above horizon. */
    void setMaximumTiltAngle(float setMaxTiltAngle);

    /** @brief Builder pattern for CameraController. */
    class Builder : public IBuilder<CameraController> {
     public:
        /** @param focusPane Pointer to pane containing camera. */
        Builder& withFocusPane(std::shared_ptr<IPane> focusPane) {
            this->impl->setFocusPane(focusPane);
            return (*this);
        }

        /**
         * @param x X-Value of new position.
         * @param y Y-Value of new position.
         * @param z Z-Value of new position.
         */
        Builder& withPosition(float x, float y, float z) {
            this->impl->setPosition(x, y, z);
            return (*this);
        }

        /** @param setPosition New position for camera. */
        Builder& withPosition(glm::vec3 setPosition) {
            this->impl->setPosition(setPosition);
            return (*this);
        }

        /** @param setTiltAngle Tilt angle, in degrees above horizon. */
        Builder& withTiltAngle(float tiltAngle) {
            this->impl->setTilt(tiltAngle);
            return (*this);
        }

        /**
         * @param setMaxTiltAngle Maximum tilt angle,
         * in degrees above horizon.
         */
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
