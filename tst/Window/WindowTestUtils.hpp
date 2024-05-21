#pragma once

#include "Window/IPane.hpp"

using basil::IPane;
using basil::PaneProps;

class TestPane : public IPane {
 public:
    explicit TestPane(PaneProps paneProps):
        IPane(paneProps) {}
    void const draw() override {
        didDraw = true;
    }
    bool didDraw = false;
};
