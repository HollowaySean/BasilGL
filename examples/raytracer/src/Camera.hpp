#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp>

namespace basil::raytracer {

enum class Chirality {
    RIGHT_HANDED, LEFT_HANDED
};

struct CameraProps {
    glm::vec3 worldUp = glm::vec3(0, 1, 0);
    Chirality chirality = Chirality::LEFT_HANDED;
    float fieldOfView = 60.0f;
    float aspectRatio = 1.0f;
    float nearClipDistance = 0.1f;
    float farClipDistance = 1000.0f;
};

class Camera {
 public:
    explicit Camera(CameraProps cameraProps = CameraProps())
    : cameraProps(cameraProps) {}

    glm::mat4 getViewMatrix();
    glm::mat4 getInverseViewMatrix();

    glm::mat4 getProjectionMatrix();
    glm::mat4 getInverseProjectionMatrix();

    void setPosition(glm::vec3 newPosition);

    void setOrientation(
        glm::vec3 newCameraUp,
        glm::vec3 newCameraForward);

    void move(glm::vec3 offset);  // NOLINT

    void pan(float turnAngle);

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
