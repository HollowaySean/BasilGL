#pragma once

#include <string>

#include <Basil/Packages/Context.hpp>

#include "Window/IPane.hpp"

namespace basil {

/** @brief Pane which runs minimal setup for using ImGui.
 *  To run a custom GUI, derive from this class and override
 *  the drawImGuiContent function with ImGui definition logic.
 *  Uses GLFW/OpenGL3 backend for ImGui. */
class ImGuiPane : public IPane,
                  private IBasilContextConsumer {
 public:
    /** @brief Runs main frame functions for ImGui. */
    virtual void draw();

 protected:
    /** @brief Sets up frame for ImGui with GLFW backend. */
    virtual void startFrame();

    /** @brief Ends frame and renders GUI content*/
    virtual void endFrame();

    /** @brief Function that runs between startFrame and
     *  endFrame. Override to set custom GUI. */
    virtual void drawImGuiContent();

    /** @brief Default title for ImGui tab. */
    std::string windowTitle = "ImGuiPane";

    /** @brief Default ImGui window flags. */
    ImGuiWindowFlags windowFlags =
          ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse;
};

}  // namespace basil
