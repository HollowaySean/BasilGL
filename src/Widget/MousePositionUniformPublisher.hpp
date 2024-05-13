#ifndef SRC_WIDGET_MOUSEPOSITIONUNIFORMPUBLISHER_HPP_
#define SRC_WIDGET_MOUSEPOSITIONUNIFORMPUBLISHER_HPP_

#include <vector>

#include <Basil/App.hpp>
#include <Basil/Widget.hpp>

namespace basil {

class MousePositionUniformPublisher : public IBasilWidget {
 public:
    void receiveData(const DataMessage& message) override {
        auto data = message.getData<UserInputModel>();
        if (data.has_value()) {
            model = data.value();
        }
    }

    void onLoop() override {
        auto mouseState = model.getMousePosition();

        float mouse_x = static_cast<float>(mouseState.xPosition);
        // TODO(sholloway): FIX THIS
        float mouse_y = -1 * static_cast<float>(mouseState.yPosition) + 400;
        float mouse_z, mouse_w;

        Logger::get().log(fmt::format("X: {}, Y: {}", mouse_x, mouse_y));

        bool isClicked = model.getIsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        bool isClicking = isClicked && !wasClicked;
        wasClicked = isClicked;

        if (isClicked) {
            lastXPosition = mouse_x;
            lastYPosition = mouse_y;
        }

        mouse_z = isClicked ? lastXPosition : -1 * lastXPosition;
        mouse_w = isClicking ? lastYPosition : -1 * lastYPosition;

        ShaderUniformModel uniformModel = ShaderUniformModel();

        std::vector<float> iMouse = { mouse_x, mouse_y, mouse_z, mouse_w };
        uniformModel.addUniformValue(iMouse, "iMouse");

        this->IDataPublisher::publishData(DataMessage(uniformModel));
    }

 private:
    UserInputModel model = UserInputModel();
    float lastXPosition, lastYPosition = 0;
    bool wasClicked = false;
};

}  // namespace basil

#endif  // SRC_WIDGET_MOUSEPOSITIONUNIFORMPUBLISHER_HPP_
