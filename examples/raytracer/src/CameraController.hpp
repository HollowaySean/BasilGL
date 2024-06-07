#pragma once

#include <memory>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/ext.hpp>

#include <Basil/App.hpp>

namespace basil::raytracer {

class CameraController : public IBasilWidget,
                         public IBuildable<CameraController> {
 public:
    CameraController();

    void onStart() override;

    void onLoop() override;

 private:
    float getDeltaTime();
    void onResize(int width, int height);

    void updatePosition(float deltaTime);
    void updateRotation(float deltaTime);
    void updateProjectionUniforms();

    UserInputWatcher userInputWatcher;
    UserInputModel& inputModel = userInputWatcher.getModel();

    float fov = 60.0f;
    float aspectRatio = 1.0f;

    // Do this better
    int sidebarWidth = 200;

    float moveSpeed = 10.0f;
    float mouseSpeed = 10.0f;

    UserInputModel::MousePosition lastMousePosition;

    FrameClock::time_point lastFrameTime;

    const glm::vec3 worldUp = glm::vec3(0, 1, 0);
    glm::vec3 cameraUp      = glm::vec3(0, 1, 0);
    glm::vec3 cameraRight   = glm::vec3(1, 0, 0);

    glm::vec3 position      = glm::vec3(0, 1, 0);
    glm::vec3 direction     = glm::vec3(0, 0, 1);

    glm::mat4 inverseView       = glm::mat4(0);
    glm::mat4 inverseProjection = glm::mat4(0);

    ShaderUniformModel uniformModel;

    unsigned int invViewID  = -1;
    unsigned int invProjID  = -1;
    unsigned int positionID = -1;

    unsigned int callbackID = -1;
};

}  // namespace basil::raytracer
