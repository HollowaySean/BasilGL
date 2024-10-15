#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <glm/ext.hpp>

namespace basil::raytracer {

/** @brief Enum indicating whether to use right- or left-handed coordinates. */
enum class Chirality {
    RIGHT_HANDED, LEFT_HANDED
};

/** @brief Container struct for camera properties. */
struct CameraProps {
    glm::vec3 worldUp = glm::vec3(0, 1, 0);
    Chirality chirality = Chirality::LEFT_HANDED;
    float fieldOfView = 60.0f;
    float aspectRatio = 1.0f;
    float nearClipDistance = 0.1f;
    float farClipDistance = 1000.0f;
};

/** @brief Class which controls camera motion and generates projection matrices. */
class Camera {
 public:
    /** @brief Initialize with given CameraProps struct. */
    explicit Camera(CameraProps cameraProps = CameraProps())
    : cameraProps(cameraProps) {}

    /** @param aspectRatio Camera aspect ratio, defined as width divided by height. */
    void setAspectRatio(float aspectRatio);

    /** @param fieldOfView Field of view, in degrees. */
    void setFieldOfView(float fieldOfView);

    /** @returns World space position of camera. */
    glm::vec3 getPosition();

    /** @returns Camera forward vector. */
    glm::vec3 getDirection();

    /** @returns World-to-camera transformation. */
    glm::mat4 getViewMatrix();

    /** @returns Camera-to-world transformation. */
    glm::mat4 getInverseViewMatrix();

    /** @returns Camera-to-screen transformation. */
    glm::mat4 getProjectionMatrix();

    /** @returns Screen-to-camera transformation. */
    glm::mat4 getInverseProjectionMatrix();

    /** @param newPosition World space position of camera. */
    void setPosition(glm::vec3 newPosition);

    /**
     * @param newCameraUp New up-direction for camera, in world space.
     * @param newCameraForward New forward-direction for camera, in world space.
     */
    void setOrientation(
        glm::vec3 newCameraUp,
        glm::vec3 newCameraForward);

    /** @param tiltAngle Camera tilt, in terms of degrees above horizon. */
    void setTiltAngle(float tiltAngle);

    /** @param offset Camera position offset, in world space. */
    void moveAbsolute(glm::vec3 offset);

    /** @param offset Camera position offset, in camera space. */
    void moveRelative(glm::vec3 offset);

    /**
     * @param rightOffset   Position offset in camera-right direction.
     * @param upOffset      Position offset in camera-up direction.
     * @param forwardOffset Position offset in camera-forward direction.
     */
    void moveRelative(
        float rightOffset,
        float upOffset,
        float forwardOffset);

    /** @param turnAngle Azimuthal angle to horizontally pivot camera, in degrees. */
    void pan(float turnAngle);

    /** @param turnAngle Elevation angle to vertically pivot camera, in degrees. */
    void tilt(float turnAngle);

 private:
    CameraProps cameraProps;

    glm::vec3 cameraUp      = glm::vec3(0, 1, 0);
    glm::vec3 cameraForward = glm::vec3(0, 0, 1);

    glm::vec3 position = glm::vec3(0);

    glm::mat4 viewMatrix = glm::mat4();
    glm::mat4 projectionMatrix = glm::mat4();
};

}  // namespace basil::raytracer
