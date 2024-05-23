#pragma once

#include <string>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/ImGui.hpp>

using basil::IBuildable;
using basil::ImGuiPane;

class SidePanel : public ImGuiPane,
                  public IBuildable<SidePanel> {
 public:
    SidePanel() {
        windowTitle = "Info";
    }

    const void drawImGuiContent() override {
        ImGui::SeparatorText("BasilGL Ray Tracing Demo");
        ImGui::TextWrapped(
            "TODO(sholloway): Write this text");
    }
};
