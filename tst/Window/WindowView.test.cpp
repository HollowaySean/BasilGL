#include <catch.hpp>

#include "GLTestUtils.hpp"
#include <Basil/Window.hpp>

TEST_CASE("Window_WindowView_WindowView") {
    SECTION("Initializes GLFW context") {
        GLFWwindow* glfwContext = glfwGetCurrentContext();
        REQUIRE(glfwContext == nullptr);

        WindowView windowView = WindowView();

        glfwContext = glfwGetCurrentContext();
        REQUIRE_FALSE(glfwContext == nullptr);
    }
}

TEST_CASE("Window_WindowView_createGLFWWindow") {
    Logger& logger = Logger::get();
    logger.clearTestInfo();

    SECTION("Creates glfwWindow object") {
        WindowView windowView = WindowView();

        GLFWwindow* window = windowView.createGLFWWindow();

        REQUIRE(window != nullptr);
        REQUIRE(logger.getLastLevel() == Level::INFO);
    }

    SECTION("Logs error on failure") {
        WindowView windowView = WindowView();
        glfwTerminate();

        GLFWwindow* window = windowView.createGLFWWindow();

        REQUIRE(window == nullptr);
        REQUIRE(logger.getLastLevel() == Level::ERROR);
    }
}

TEST_CASE("Window_WindowView_logGLFWError") {
    Logger& logger = Logger::get();
    logger.clearTestInfo();

    GLTestUtils::initializeGLContext();

    SECTION("Logs info on success") {
        WindowView::logGLFWError(1);

        REQUIRE(logger.getLastOutput() ==
            "[INFO]: GLFW context initialized successfully.\n");
        REQUIRE(logger.getLastLevel() ==
            Level::INFO);
    }

    SECTION("Logs error on failure") {
        WindowView::logGLFWError(0);

        REQUIRE(logger.getLastLevel() == Level::ERROR);
    }

    GLTestUtils::deinitialize();
}

TEST_CASE("Window_WindowView_logGLEWError") {
    Logger& logger = Logger::get();
    logger.clearTestInfo();

    SECTION("Logs info on success") {
        WindowView::logGLEWError(GLEW_OK);

        REQUIRE(logger.getLastOutput() ==
            "[INFO]: GLEW context initialized successfully.\n");
        REQUIRE(logger.getLastLevel() ==
            Level::INFO);
    }

    SECTION("Logs error on failure") {
        WindowView::logGLEWError(!GLEW_OK);

        REQUIRE(logger.getLastLevel() == Level::ERROR);
    }
}

TEST_CASE("Window_WindowView_getTopPaneProps") {
    SECTION("Returns PaneProps with window size.") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        WindowOptions windowProps = window.windowOptions;

        REQUIRE(paneProps.width == windowProps.width);
        REQUIRE(paneProps.height == windowProps.height);
        REQUIRE(paneProps.xOffset == 0);
        REQUIRE(paneProps.yOffset == 0);
    }
}
