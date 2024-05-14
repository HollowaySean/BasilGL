#ifndef SRC_WIDGET_MOUSEPOSITIONUNIFORMPUBLISHER_HPP_
#define SRC_WIDGET_MOUSEPOSITIONUNIFORMPUBLISHER_HPP_

#include <vector>

#include <Basil/App.hpp>
#include <Basil/Widget.hpp>

namespace basil {

class MousePositionUniformPublisher : public IBasilWidget {
 public:
    MousePositionUniformPublisher()
        : IBasilWidget(ProcessOrdinal::EARLY, ProcessPrivilege::NONE,
            "MousePositionUniformPublisher") {}

    void receiveData(const DataMessage& message) override {
        auto data = message.getData<UserInputModel>();
        if (data.has_value()) {
            model = data.value();
        }
    }

    void onLoop() override {
        auto mouseState = model.getMousePosition();
        auto windowSize = model.getWindowSize();

        float resolution_x = static_cast<float>(windowSize.width);
        float resolution_y = static_cast<float>(windowSize.height);

        float raw_x = static_cast<float>(mouseState.xPosition);
        float raw_y;
        if (INVERT_Y_AXIS) {
            raw_y = resolution_y - static_cast<float>(mouseState.yPosition);
        } else {
            raw_y = static_cast<float>(mouseState.yPosition);
        }

        bool isClicking = model.getIsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
        bool isClickStart = isClicking && !wasClicking;

        if (isClickStart) {
            lastStart_x = raw_x;
            lastStart_y = raw_y;
        }

        if (isClicking) {
            lastDown_x = raw_x;
            lastDown_y = raw_y;
        }
        wasClicking = isClicking;

        ShaderUniformModel uniformModel = ShaderUniformModel();

        std::vector<float> iMouse =
            {   lastDown_x,
                lastDown_y,
                (isClicking   ? 1 : -1) * lastStart_x,
                (isClickStart ? 1 : -1) * lastStart_y
            };

        uniformModel.addUniformValue(iMouse, "iMouse");

        // TODO(sholloway): Rename class if also publishing this
        // Also add explanation of third value
        std::vector<float> iResolution = { resolution_x, resolution_y, 1.0 };
        uniformModel.addUniformValue(iResolution, "iResolution");

        this->IDataPublisher::publishData(DataMessage(uniformModel));
    }

 private:
    UserInputModel model = UserInputModel();
    float lastDown_x = 0;
    float lastDown_y = 0;
    float lastStart_x = 0;
    float lastStart_y = 0;
    bool wasClicking = false;

    bool INVERT_Y_AXIS = BASIL_INVERT_CURSOR_Y_AXIS;
};

}  // namespace basil

#endif  // SRC_WIDGET_MOUSEPOSITIONUNIFORMPUBLISHER_HPP_
