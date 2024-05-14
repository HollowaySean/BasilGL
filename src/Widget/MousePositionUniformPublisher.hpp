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

        // TODO(sholloway): FIX THIS
        float raw_x = static_cast<float>(mouseState.xPosition);
        float raw_y = 400 - static_cast<float>(mouseState.yPosition);
        bool isClicking = model.getIsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);

        if (isClicking) {
            if (!wasClicking) {
                lastStart_x = raw_x;
                lastStart_y = raw_y;
            }

            lastDown_x = raw_x;
            lastDown_y = raw_y;
        }
        wasClicking = isClicking;

        ShaderUniformModel uniformModel = ShaderUniformModel();

        std::vector<float> iMouse =
            {   lastDown_x,
                lastDown_y,
                (isClicking ? 1 : -1) * lastStart_x,
                (isClicking ? 1 : -1) * lastStart_y
            };

        uniformModel.addUniformValue(iMouse, "iMouse");

        this->IDataPublisher::publishData(DataMessage(uniformModel));
    }

 private:
    UserInputModel model = UserInputModel();
    float lastDown_x, lastDown_y, lastStart_x, lastStart_y;
    bool wasClicking = false;
};

}  // namespace basil

#endif  // SRC_WIDGET_MOUSEPOSITIONUNIFORMPUBLISHER_HPP_
