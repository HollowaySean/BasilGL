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

template<class T>
using s_pt = std::shared_ptr<T>;

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

TEST_CASE("Window_WindowView_setWindowProps") {
    SECTION("Sets windowProps to new values") {
        WindowView window = WindowView();
        WindowProps newProps = WindowProps {
            .title = "test",
            .width = 55,
            .height = 22
        };

        window.setWindowProps(newProps);

        REQUIRE(window.windowProps.title == newProps.title);
        REQUIRE(window.windowProps.width == newProps.width);
        REQUIRE(window.windowProps.height == newProps.height);
    }
}

TEST_CASE("Window_WindowView_setWindowTitle") {
    SECTION("Sets GLFW window title") {
        WindowView window = WindowView();

        std::string newTitle = "newTitle";
        window.setWindowTitle(newTitle);

        REQUIRE(window.windowProps.title == newTitle);

        const char* glfwTitle = glfwGetWindowTitle(window.glfwWindow);
        REQUIRE(std::string(glfwTitle) == newTitle);
    }
}

TEST_CASE("Window_WindowView_setWindowSize") {
    SECTION("Sets GLFW window title") {
        WindowView window = WindowView();

        int newWidth = 55;
        int newHeight = 22;
        window.setWindowSize(newWidth, newHeight);

        REQUIRE(window.windowProps.width == newWidth);
        REQUIRE(window.windowProps.height == newHeight);

        // TODO(sholloway): Reinstate after
        //  achieving thread safety for GLFW context
        // int actualWidth, actualHeight;
        // glfwGetWindowSize(window.glfwWindow, &actualWidth, &actualHeight);
        // REQUIRE(actualWidth == newWidth);
        // REQUIRE(actualHeight == newHeight);
    }
}

TEST_CASE("Window_WindowView_draw") {
    SECTION("Calls draw function on top pane") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        s_pt<TestPane> testPane = std::make_shared<TestPane>(paneProps);

        window.draw();
        REQUIRE_FALSE(testPane->didDraw);
        window.setTopPane(testPane);
        window.draw();
        REQUIRE(testPane->didDraw);
    }
}

TEST_CASE("Window_WindowView_onLoop") {
    SECTION("Calls draw function") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        s_pt<TestPane> testPane = std::make_shared<TestPane>(paneProps);

        REQUIRE_FALSE(testPane->didDraw);
        window.setTopPane(testPane);
        window.onLoop();
        REQUIRE(testPane->didDraw);
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

        window.onStop();
        REQUIRE(logger.getLastLevel() == LogLevel::INFO);
        REQUIRE(logger.getLastOutput() == "Stopping loop");
    }
}

TEST_CASE("Window_WindowView_onResize") {
    SECTION("Calls resize function on top pane") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        s_pt<TestPane> testPane = std::make_shared<TestPane>(paneProps);

        window.setTopPane(testPane);

        window.onResize(50, 25);
        REQUIRE(testPane->paneProps.width == 50);
        REQUIRE(testPane->paneProps.height == 25);
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

TEST_CASE("Window_WindowView_Builder") {
    SECTION("Builds WindowView object") {
        std::string title = "testTitle";
        int width = 54;
        int height = 23;
        auto pane = std::make_shared<TestPane>(PaneProps());

        auto window = WindowView::Builder()
            .withDimensions(width, height)
            .withTitle(title)
            .withTopPane(pane)
            .build();

        REQUIRE(window->windowProps.title == title);
        REQUIRE(window->windowProps.width == width);
        REQUIRE(window->windowProps.height == height);
        REQUIRE(window->topPane == pane);
    }
}
