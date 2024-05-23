#pragma once

#include "Window/IPane.hpp"

using basil::IPane;
using basil::ViewArea;

class TestPane : public IPane {
 public:
    explicit TestPane(ViewArea viewArea):
        IPane(viewArea) {}
    void const draw() override {
        didDraw = true;
    }
    bool didDraw = false;
};
