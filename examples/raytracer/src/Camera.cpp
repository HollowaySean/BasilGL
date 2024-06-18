#include "Camera.hpp"

namespace basil::raytracer {

glm::mat4 Camera::getViewMatrix() {
    switch (cameraProps.chirality) {
        case Chirality::RIGHT_HANDED:
            return glm::lookAtRH(
                position,
                position + cameraForward,
                cameraUp);

        case Chirality::LEFT_HANDED:
        default:
            return glm::lookAtLH(
                position,
                position + cameraForward,
                cameraUp);
    }
}

glm::mat4 Camera::getInverseViewMatrix() {
    return glm::inverse(getViewMatrix());
}

glm::mat4 Camera::getProjectionMatrix() {
    switch (cameraProps.chirality) {
        case Chirality::RIGHT_HANDED:
            return glm::perspectiveRH(
                glm::radians(cameraProps.fieldOfView),
                cameraProps.aspectRatio,
                cameraProps.nearClipDistance,
                cameraProps.farClipDistance);

        case Chirality::LEFT_HANDED:
        default:
            return glm::perspectiveLH(
                glm::radians(cameraProps.fieldOfView),
                cameraProps.aspectRatio,
                cameraProps.nearClipDistance,
                cameraProps.farClipDistance);
    }
}

glm::mat4 Camera::getInverseProjectionMatrix() {
    return glm::inverse(getProjectionMatrix());
}

void Camera::setPosition(glm::vec3 newPosition) {
    position = newPosition;
}

void Camera::setOrientation(
        glm::vec3 newCameraUp, glm::vec3 newCameraForward) {
    cameraUp = newCameraUp;
    cameraForward = newCameraForward;
}

void Camera::move(glm::vec3 offset) {  // NOLINT
    setPosition(position + offset);
}

void Camera::pan(float turnAngle) {
    cameraForward = glm::rotate(
        cameraForward,
        glm::radians(turnAngle),
        cameraProps.worldUp);
}

void Camera::tilt(float turnAngle) {
    cameraForward = glm::rotate(
        cameraForward,
        glm::radians(turnAngle),
        glm::cross(cameraUp, cameraForward));
}

}  // namespace basil::raytracer
