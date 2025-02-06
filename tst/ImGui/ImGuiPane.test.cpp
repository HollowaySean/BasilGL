#include <catch.hpp>

#if BASIL_INCLUDE_IMGUI

#include "OpenGL/GLTestUtils.hpp"

#include "ImGui/ImGuiPane.hpp"

using basil::ImGuiPane;

TEST_CASE("ImGui_ImGuiPane_draw") { BASIL_LOCK_TEST
    class TestImGuiPane : public ImGuiPane {
     public:
        void drawImGuiContent() override {
            this->ImGuiPane::drawImGuiContent();

            ImVec2 size = ImGui::GetWindowSize();
            ImVec2 position = ImGui::GetWindowPos();

            CHECK(size.x == 100);
            CHECK(size.y == 200);
            CHECK(position.x == 5);
            CHECK(position.y == 20);
        }
    };

    auto pane = TestImGuiPane();
    pane.setPaneProps({
        100, 200, 5, 20
    });

    SECTION("Fits ImGui window to Pane") {
        pane.draw();
    }
}

#endif  // BASIL_INCLUDE_IMGUI
