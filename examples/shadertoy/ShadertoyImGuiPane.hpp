#pragma once

#include <string>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/ImGui.hpp>

using basil::IBuildable;
using basil::ImGuiPane;

class ShadertoyImGuiPane : public ImGuiPane,
                           public IBuildable<ShadertoyImGuiPane> {
 public:
    ShadertoyImGuiPane() {
        windowTitle = "Overview";
    }

    const void drawImGuiContent() override {
        ImGui::SeparatorText("BasilGL Shadertoy Demo");
        ImGui::TextWrapped(
            "This example demonstrates the use of Basil's "
            "BasilApp and builder patterns to quickly build a "
            "simple clone of Shadertoy.\n"
            "\nFeatures include:");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "Automatic translation of shadertoy "
            "formatted shaders into GLSL format");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "Built-in support for simple shadertoy uniforms: "
            "iResolution, iMouse, iTime, "
            "iDeltaTime, iFrame, and iFrameRate");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "Hot reloading of shader code (Try editing \"test.shadertoy\")");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "Automatic propagation of shader uniforms "
            "from JSON file, with hot reloading");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "Image capture (Press S to take a screenshot)");
    }
};
