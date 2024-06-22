#include "Camera.hpp"

namespace basil::raytracer {

void Camera::setAspectRatio(float aspectRatio) {
    cameraProps.aspectRatio = aspectRatio;
}

void Camera::setFieldOfView(float fieldOfView) {
    cameraProps.fieldOfView = fieldOfView;
}

glm::vec3 Camera::getPosition() {
    return position;
}

glm::vec3 Camera::getDirection() {
    return cameraForward;
}

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

void Camera::moveAbsolute(glm::vec3 offset) {
    setPosition(position + offset);
}

void Camera::moveRelative(glm::vec3 offset) {
    moveRelative(offset.x, offset.y, offset.z);
}

void Camera::moveRelative(
        float rightOffset, float upOffset, float forwardOffset) {
    glm::vec3 offset = glm::vec3(0);
    offset += forwardOffset * cameraForward;
    offset += upOffset * cameraProps.worldUp;
    offset += rightOffset * glm::cross(cameraUp, cameraForward);

    moveAbsolute(offset);
}


void Camera::pan(float turnAngle) {
    cameraForward = glm::rotate(
        cameraForward,
        glm::radians(turnAngle),
        cameraProps.worldUp);
}

void Camera::tilt(float turnAngle) {
    if (cameraProps.chirality == Chirality::LEFT_HANDED) {
        turnAngle *= -1;
    }

    cameraForward = glm::rotate(
        cameraForward,
        glm::radians(turnAngle),
        glm::cross(cameraUp, cameraForward));
}

}  // namespace basil::raytracer
