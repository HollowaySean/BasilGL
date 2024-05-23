#pragma once

#include <string>

#include <Basil/Packages/Builder.hpp>
#include <Basil/Packages/ImGui.hpp>

using basil::IBuildable;
using basil::ImGuiPane;

class ImGuiExamplePane : public ImGuiPane,
                         public IBuildable<ImGuiExamplePane> {
 public:
    ImGuiExamplePane() {
        windowTitle = "Info";
    }

    const void drawImGuiContent() override {
        ImGui::SeparatorText("BasilGL Panes Demo");
        ImGui::TextWrapped(
            "This example demonstrates the use of Basil's "
            "builder patterns to nest multiple shader panes "
            "following different rules.\n"
            "\nRules for each pane:");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "This ImGui pane: Fixed at 300 pixel width");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "Red shader pane: Fixed at 1:1 aspect ratio");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "Green shader pane: Fixed at 75%% vertical extent");
        ImGui::Bullet();
        ImGui::TextWrapped(
            "Yellow & blue shader panes: Fixed at 50%% horizontal extent");
    }
};
