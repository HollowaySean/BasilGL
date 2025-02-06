#pragma once

#include <string>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/ImGui.hpp>

namespace basil::raytracer {

/** @brief Side panel, which implements ImGuiPane. */
class SidePanel : public ImGuiPane,
                  public IBuildable<SidePanel> {
 public:
    /** @brief Initialize pane info. */
    SidePanel() {
        windowTitle = "BasilGL Ray Tracing Demo";
    }

    /** @brief Run ImGui processes. */
    void drawImGuiContent() override {
        ImGui::SeparatorText("Info");
        ImGui::TextWrapped(
            "This example demonstrates a simple raytracer project built "
            "with BasilGL.\n");

        ImGui::SeparatorText("Controls");

        ImGui::Bullet();
        ImGui::TextWrapped("Mouse - Adjust camera direction.");
        ImGui::Bullet();
        ImGui::TextWrapped("WASD - Move camera laterally.");
        ImGui::Bullet();
        ImGui::TextWrapped("QE - Move camera vertically.");
        ImGui::Bullet();
        ImGui::TextWrapped("F - Save screenshot to file.");


        ImGui::SeparatorText("Render Settings");
        ImGui::TextWrapped(
            "Settings are defined in assets/uniforms.json file. "
            "Updated uniforms are hot reloaded on file save.");
    }
};

}  // namespace basil::raytracer
