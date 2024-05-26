#pragma once

#include <glm/glm.hpp>

#include <Basil/App.hpp>

using basil::IBasilWidget;
using basil::IBuildable;

class CameraWidget : public IBasilWidget,
                     public IBuildable<CameraWidget> {
 public:
};
