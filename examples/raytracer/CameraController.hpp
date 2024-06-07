#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Basil/App.hpp>

using basil::DataMessage;
using basil::GLUniformPointer;
using basil::IBasilWidget;
using basil::IBuildable;
using basil::ShaderUniformModel;

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
        glm::vec3 worldUp = glm::vec3(0, 1, 0);
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
            glm::radians(60.0f), 1.0f, 0.1f, 1000.0f);
        inverseProjection = glm::inverse(project);

        uniformModel.addUniform(
            std::make_shared<GLUniformPointer<float>>(
                glm::value_ptr(inverseView),
                    "inverseView", 4, 4, 1));

        uniformModel.addUniform(
            std::make_shared<GLUniformPointer<float>>(
                glm::value_ptr(inverseProjection),
                    "inverseProjection", 4, 4, 1));

        uniformModel.addUniform(
            std::make_shared<GLUniformPointer<float>>(
                glm::value_ptr(position),
                    "position", 3, 1, 1));

        publishData(DataMessage(uniformModel));
    }

    void onLoop() override {}

 private:
    glm::vec3 position = glm::vec3(0, 1, 0);
    glm::vec3 direction = glm::vec3(0, 0, 1);

    glm::mat4 inverseView = glm::mat4(0);
    glm::mat4 inverseProjection = glm::mat4(0);

    ShaderUniformModel uniformModel;
};
