#include <catch.hpp>

#include "GLTestUtils.hpp"

#include "Window/HotReloadShaderPane.hpp"

using basil::HotReloadShaderPane;
using basil::PaneProps;

TEST_CASE("Window_HotReloadShaderPane_HotReloadShaderPane") { BASIL_LOCK_TEST
    SECTION("Loads shader from file if compilable") {
        auto pane = HotReloadShaderPane(paneProps, fragmentPath);

        CHECK(pane.currentShaderProgram->fragmentShader
             != pane.defaultFrag);
    }

    SECTION("Loads debug shader if file can not be compiled") {
        auto pane = HotReloadShaderPane(paneProps, badFragmentPath);

        CHECK(pane.currentShaderProgram->fragmentShader
             == pane.defaultFrag);
    }

    SECTION("Loads debug shader if file is not found") {
        auto pane = HotReloadShaderPane(paneProps, invalidPath);

        CHECK(pane.currentShaderProgram->fragmentShader
             == pane.defaultFrag);
    }
}

TEST_CASE("Window_HotReloadShaderPane_draw") { BASIL_LOCK_TEST
    auto pane = HotReloadShaderPane(paneProps, invalidPath);

    SECTION("Updates the shader if modified") {
        CHECK(pane.currentShaderProgram->getFragmentShader()
            == pane.defaultFrag);

        pane.timestamp = std::filesystem::file_time_type::min();
        pane.filePath = fragmentPath;
        pane.draw();

        CHECK(pane.currentShaderProgram->getFragmentShader()
            != pane.defaultFrag);
    }
}

TEST_CASE("Window_HotReloadShaderPane_Builder") { BASIL_LOCK_TEST
    SECTION("Builds from file path") {
        auto pane = HotReloadShaderPane::Builder()
            .fromFilePath(fragmentPath)
            .withPaneProps(paneProps)
            .build();

        CHECK(pane->currentShaderProgram->fragmentShader
            != pane->defaultFrag);
        CHECK(pane->paneProps.width == 20);
        CHECK(pane->paneProps.height == 10);
    }
}
