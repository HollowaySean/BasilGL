#ifndef SRC_WIDGET_SHADERTOYUNIFORMPUBLISHER_HPP_
#define SRC_WIDGET_SHADERTOYUNIFORMPUBLISHER_HPP_

#include <memory>
#include <string>
#include <vector>

#include <Basil/App.hpp>

#include "SystemTimeWatcher.hpp"
#include "UserInputWatcher.hpp"

namespace basil {

class ShadertoyUniformPublisher : public IBasilWidget {
 public:
    ShadertoyUniformPublisher();

    void onStart() override;

    void onLoop() override;

 private:
    void initializeUniforms();

    void setResolution();
    void setMouse();
    void setTime();

    UserInputWatcher inputWatcher = UserInputWatcher();
    UserInputModel& inputModel = inputWatcher.getModel();

    SystemTimeWatcher timeWatcher = SystemTimeWatcher();
    SystemTimeModel& timeModel = timeWatcher.getModel();

    ShaderUniformModel uniformModel = ShaderUniformModel();

    float   lastDown_x = 0, lastDown_y = 0, lastStart_x = 0,
            lastStart_y = 0, resolution_x = 0, resolution_y = 0;

    unsigned int resolutionID = 0, mouseID = 0, timeID = 0,
                 deltaTimeID = 0, frameRateID = 0;

    bool wasClicking = false;

    float PIXEL_ASPECT_RATIO = BASIL_PIXEL_ASPECT_RATIO;
    bool INVERT_Y_AXIS = BASIL_INVERT_CURSOR_Y_AXIS;

    static inline const std::string RESOLUTION_UNIFORM_NAME = "iResolution";
    static inline const std::string MOUSE_UNIFORM_NAME = "iMouse";
    static inline const std::string TIME_UNIFORM_NAME = "iTime";
    static inline const std::string DELTATIME_UNIFORM_NAME = "iDeltaTime";
    static inline const std::string FRAMERATE_UNIFORM_NAME = "iFrameRate";
};

}  // namespace basil

#endif  // SRC_WIDGET_SHADERTOYUNIFORMPUBLISHER_HPP_
