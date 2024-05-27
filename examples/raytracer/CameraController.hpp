#pragma once

#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Basil/App.hpp>

using basil::IBasilWidget;
using basil::IBuildable;
using basil::ShaderUniformModel;

class CameraController : public IBasilWidget,
                         public IBuildable<CameraController> {
 public:
    void onStart() override {
        glm::vec3 worldUp = glm::vec3(0, 1, 0);
        glm::vec3 cameraRight = glm::normalize(
            glm::cross(worldUp, direction));
        glm::vec3 cameraUp =
            glm::cross(direction, cameraRight);

        glm::mat4 view = glm::lookAt(
            position + glm::vec3(0, 0, 1.0f),
            position,
            worldUp);

        // std::vector<float> data = glm::value_ptr(view);
        // uniformModel.addUniform();
    }

    void onLoop() override {}

 private:
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 direction = glm::vec3(1, 0, 0);

    ShaderUniformModel uniformModel;
};
