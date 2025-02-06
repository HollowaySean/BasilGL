#pragma once

#include "Window/IPane.hpp"

using basil::IPane;
using basil::ViewArea;

class TestPane : public IPane {
 public:
    explicit TestPane(ViewArea viewArea):
        IPane(viewArea) {}
    void draw() override {
        didDraw = true;
    }
    bool didDraw = false;
};

inline const ViewArea testViewArea = {
    .width = 200,
    .height = 100,
    .xOffset = 5,
    .yOffset = 2
};
