#include <catch.hpp>

#include <Basil/Context.hpp>
#include <Basil/Window.hpp>

using basil::BasilContext;
using basil::Logger;
using basil::LogLevel;
using basil::IPane;
using basil::PaneProps;
using basil::WindowProps;
using basil::WindowView;
using basil::IFrameProcess;

class TestPane : public IPane {
 public:
    explicit TestPane(PaneProps paneProps):
        IPane(paneProps) {}
    void const draw() override {
        didDraw = true;
    }
    bool didDraw = false;
};

TEST_CASE("Window_WindowView_WindowView") {
    SECTION("Initializes GLFW context") {
        BasilContext::terminate();

        GLFWwindow* glfwContext = glfwGetCurrentContext();
        REQUIRE(glfwContext == nullptr);

        WindowProps windowProps = WindowProps {
            .title = "test name",
            .width = 100,
            .height = 50
        };
        WindowView windowView = WindowView(windowProps);

        glfwContext = glfwGetCurrentContext();
        REQUIRE_FALSE(glfwContext == nullptr);
    }

    SECTION("Uses default WindowProps if none provided") {
        WindowView windowView = WindowView();
        WindowProps windowProps = windowView.windowProps;

        REQUIRE(windowProps.width == WindowProps::DEFAULT_WIDTH);
        REQUIRE(windowProps.height == WindowProps::DEFAULT_HEIGHT);
        REQUIRE(windowProps.title == WindowProps::DEFAULT_TITLE);
    }
}

TEST_CASE("Window_WindowView_createGLFWWindow") {
    Logger& logger = Logger::get();
    logger.clearTestInfo();

    SECTION("Creates glfwWindow object") {
        WindowView windowView = WindowView();

        GLFWwindow* window = windowView.createGLFWWindow();

        REQUIRE(window != nullptr);
        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
    }

    SECTION("Logs error on failure") {
        WindowView windowView = WindowView();
        glfwTerminate();

        GLFWwindow* window = windowView.createGLFWWindow();

        REQUIRE(window == nullptr);
        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
    }
}

TEST_CASE("Window_WindowView_getTopPaneProps") {
    SECTION("Returns PaneProps with window size.") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        WindowProps windowProps = window.windowProps;

        REQUIRE(paneProps.width == windowProps.width);
        REQUIRE(paneProps.height == windowProps.height);
        REQUIRE(paneProps.xOffset == 0);
        REQUIRE(paneProps.yOffset == 0);
    }
}

TEST_CASE("Window_WindowView_draw") {
    SECTION("Calls draw function on top pane") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        TestPane testPane = TestPane(paneProps);

        window.draw();
        REQUIRE_FALSE(testPane.didDraw);
        window.setTopPane(&testPane);
        window.draw();
        REQUIRE(testPane.didDraw);
    }
}

TEST_CASE("Window_WindowView_onLoop") {
    SECTION("Calls draw function") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        TestPane testPane = TestPane(paneProps);

        REQUIRE_FALSE(testPane.didDraw);
        window.setTopPane(&testPane);
        window.onLoop();
        REQUIRE(testPane.didDraw);
    }

    SECTION("Closes window if requested by GLFW") {
        WindowView window = WindowView();

        glfwSetWindowShouldClose(window.glfwWindow, GLFW_TRUE);
        window.onLoop();

        REQUIRE(window.currentState == IFrameProcess::State::REQUEST_STOP);

        GLFWwindow* glfwContext = glfwGetCurrentContext();
        REQUIRE(glfwContext == nullptr);
    }
}

TEST_CASE("Window_WindowView_onStart") {
    SECTION("Makes window visible") {
        WindowView window = WindowView();

        int visible = glfwGetWindowAttrib(window.glfwWindow, GLFW_VISIBLE);
        REQUIRE(visible == GLFW_FALSE);

        window.onStart();
        visible = glfwGetWindowAttrib(window.glfwWindow, GLFW_VISIBLE);
        REQUIRE(visible == GLFW_TRUE);
    }
}

TEST_CASE("Window_WindowView_onStop") {
    Logger& logger = Logger::get();

    SECTION("Logs closing message") {
        WindowView window = WindowView();
        logger.clearTestInfo();

        window.onStop();
        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == "[INFO]: Stopping loop\n");
    }
}

TEST_CASE("Window_WindowView_onResize") {
    SECTION("Calls resize function on top pane") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        TestPane testPane = TestPane(paneProps);
        window.setTopPane(&testPane);

        window.onResize(50, 25);
        REQUIRE(testPane.paneProps.width == 50);
        REQUIRE(testPane.paneProps.height == 25);
    }
}

TEST_CASE("Window_WindowView_resizeCallback") {
    SECTION("Calls resize function on window") {
        WindowView window = WindowView();
        WindowView::resizeCallback(window.glfwWindow, 60, 30);

        REQUIRE(window.windowProps.width == 60);
        REQUIRE(window.windowProps.height == 30);
    }
}
