#include <catch.hpp>

#include "OpenGL/HotReloadShaderPane.hpp"

#include "OpenGL/GLTestUtils.hpp"

using basil::HotReloadShaderPane;
using basil::ViewArea;

TEST_CASE("OpenGL_HotReloadShaderPane_HotReloadShaderPane") { BASIL_LOCK_TEST
    SECTION("Loads shader from file if compilable") {
        auto pane = HotReloadShaderPane(testViewArea, fragmentPath);

        CHECK(pane.currentShaderProgram->fragmentShader
             != pane.defaultFrag);
    }

    SECTION("Loads debug shader if file can not be compiled") {
        auto pane = HotReloadShaderPane(testViewArea, badFragmentPath);

        CHECK(pane.currentShaderProgram->fragmentShader
             == pane.defaultFrag);
    }

    SECTION("Loads debug shader if file is not found") {
        auto pane = HotReloadShaderPane(testViewArea, invalidPath);

        CHECK(pane.currentShaderProgram->fragmentShader
             == pane.defaultFrag);
    }
}

TEST_CASE("OpenGL_HotReloadShaderPane_draw") { BASIL_LOCK_TEST
    auto pane = HotReloadShaderPane(testViewArea, invalidPath);

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

TEST_CASE("OpenGL_HotReloadShaderPane_Builder") { BASIL_LOCK_TEST
    SECTION("Builds from file path") {
        auto pane = HotReloadShaderPane::Builder()
            .fromFilePath(fragmentPath)
            .build();

        CHECK(pane->currentShaderProgram->fragmentShader
            != pane->defaultFrag);
    }
}
