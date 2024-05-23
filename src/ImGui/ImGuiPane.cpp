#if BASIL_INCLUDE_IMGUI

#include "ImGuiPane.hpp"

namespace basil {

const void ImGuiPane::draw() {
    startFrame();
    drawImGuiContent();
    endFrame();
}

void ImGuiPane::startFrame() {
    // Begins ImGui frame using GLFW backend
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Fixes ImGui window to pane size & position
    ImVec2 position = ImVec2(viewArea.xOffset, viewArea.yOffset);
    ImVec2 size = ImVec2(viewArea.width, viewArea.height);
    ImGui::SetNextWindowPos(position, ImGuiCond_Always);
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);

    // Start GUI definition
    ImGui::Begin(windowTitle.c_str(), NULL, windowFlags);
}

void ImGuiPane::endFrame() {
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

const void ImGuiPane::drawImGuiContent() {
    ImGui::Text("Sample ImGui content.");
}

}  // namespace basil

#endif  // BASIL_INCLUDE_IMGUI
