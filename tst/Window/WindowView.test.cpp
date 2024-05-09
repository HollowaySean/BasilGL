#include <catch.hpp>

#include <Basil/Context.hpp>
#include <Basil/Window.hpp>

#include "GLTestUtils.hpp"

using basil::BasilContext;
using basil::BasilContextLock;
using basil::Logger;
using basil::LogLevel;
using basil::IPane;
using basil::PaneProps;
using basil::ProcessState;
using basil::WindowProps;
using basil::WindowView;

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

TEST_CASE("Window_WindowView_WindowView") { BASIL_LOCK_TEST
    SECTION("Initializes GLFW context") {
        BasilContext::terminate();

        GLFWwindow* glfwContext = glfwGetCurrentContext();
        CHECK(glfwContext == nullptr);

        WindowProps windowProps = WindowProps {
            .title = "test name",
            .width = 100,
            .height = 50
        };
        WindowView windowView = WindowView(windowProps);

        glfwContext = glfwGetCurrentContext();
        CHECK_FALSE(glfwContext == nullptr);
    }

    SECTION("Uses default WindowProps if none provided") {
        WindowView windowView = WindowView();
        WindowProps windowProps = windowView.windowProps;

        CHECK(windowProps.width == WindowProps::DEFAULT_WIDTH);
        CHECK(windowProps.height == WindowProps::DEFAULT_HEIGHT);
        CHECK(windowProps.title == WindowProps::DEFAULT_TITLE);
    }
}

TEST_CASE("Window_WindowView_getTopPaneProps") {
    SECTION("Returns PaneProps with window size.") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        WindowProps windowProps = window.windowProps;

        CHECK(paneProps.width == windowProps.width);
        CHECK(paneProps.height == windowProps.height);
        CHECK(paneProps.xOffset == 0);
        CHECK(paneProps.yOffset == 0);
    }
}

TEST_CASE("Window_WindowView_setWindowProps") { BASIL_LOCK_TEST
    SECTION("Sets windowProps to new values") {
        WindowView window = WindowView();
        WindowProps newProps = WindowProps {
            .title = "test",
            .width = 55,
            .height = 22
        };

        window.setWindowProps(newProps);

        CHECK(window.windowProps.title == newProps.title);
        CHECK(window.windowProps.width == newProps.width);
        CHECK(window.windowProps.height == newProps.height);
    }
}

TEST_CASE("Window_WindowView_setWindowTitle") { BASIL_LOCK_TEST
    SECTION("Sets GLFW window title") {
        WindowView window = WindowView();

        std::string newTitle = "newTitle";
        window.setWindowTitle(newTitle);

        CHECK(window.windowProps.title == newTitle);

        const char* glfwTitle = glfwGetWindowTitle(window.glfwWindow);
        CHECK(std::string(glfwTitle) == newTitle);
    }
}

TEST_CASE("Window_WindowView_setWindowSize") { BASIL_LOCK_TEST
    SECTION("Sets GLFW window dimensions") {
        WindowView window = WindowView();

        int newWidth = 55;
        int newHeight = 22;
        window.setWindowSize(newWidth, newHeight);

        CHECK(window.windowProps.width == newWidth);
        CHECK(window.windowProps.height == newHeight);

        // glfwGetWindowSize seems to CHECK at least
        //  one frame to pass before update
        std::this_thread::sleep_for(
            std::chrono::milliseconds(100));

        int actualWidth, actualHeight;
        glfwGetWindowSize(window.glfwWindow, &actualWidth, &actualHeight);

        CHECK(actualWidth == newWidth);
        CHECK(actualHeight == newHeight);
    }
}

TEST_CASE("Window_WindowView_draw") {
    SECTION("Calls draw function on top pane") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        s_pt<TestPane> testPane = std::make_shared<TestPane>(paneProps);

        window.draw();
        CHECK_FALSE(testPane->didDraw);
        window.setTopPane(testPane);
        window.draw();
        CHECK(testPane->didDraw);
    }
}

TEST_CASE("Window_WindowView_onLoop") {
    SECTION("Calls draw function") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        s_pt<TestPane> testPane = std::make_shared<TestPane>(paneProps);

        CHECK_FALSE(testPane->didDraw);
        window.setTopPane(testPane);
        window.onLoop();
        CHECK(testPane->didDraw);
    }

    SECTION("Closes window if requested by GLFW") {
        BASIL_LOCK_TEST

        WindowView window = WindowView();

        glfwSetWindowShouldClose(window.glfwWindow, GLFW_TRUE);
        window.onLoop();

        CHECK(window.currentState == ProcessState::REQUEST_STOP);

        GLFWwindow* glfwContext = glfwGetCurrentContext();
        CHECK(glfwContext == nullptr);
    }
}

TEST_CASE("Window_WindowView_onStart") { BASIL_LOCK_TEST
    SECTION("Makes window visible") {
        WindowView window = WindowView();

        int visible = glfwGetWindowAttrib(window.glfwWindow, GLFW_VISIBLE);
        CHECK(visible == GLFW_FALSE);

        window.onStart();
        visible = glfwGetWindowAttrib(window.glfwWindow, GLFW_VISIBLE);
        CHECK(visible == GLFW_TRUE);
    }
}

TEST_CASE("Window_WindowView_onStop") {
    Logger& logger = Logger::get();

    SECTION("Logs closing message") {
        WindowView window = WindowView();

        window.onStop();
        CHECK(logger.getLastLevel() == LogLevel::INFO);
        CHECK(logger.getLastOutput() == "Stopping loop");
    }
}

TEST_CASE("Window_WindowView_setTopPane") {
    auto window = WindowView();
    auto props = window.getTopPaneProps();
    auto pane = std::make_shared<TestPane>(props);
    auto anotherPane = std::make_shared<TestPane>(props);

    SECTION("Subscribes/unsubscribes data subscribers") {
        window.setTopPane(pane);
        CHECK(window.subscriptions.contains(pane));

        window.setTopPane(anotherPane);
        CHECK(window.subscriptions.contains(anotherPane));
        CHECK_FALSE(window.subscriptions.contains(pane));
    }
}

TEST_CASE("Window_WindowView_onResize") {
    SECTION("Calls resize function on top pane") {
        WindowView window = WindowView();

        PaneProps paneProps = window.getTopPaneProps();
        s_pt<TestPane> testPane = std::make_shared<TestPane>(paneProps);

        window.setTopPane(testPane);

        window.onResize(50, 25);
        CHECK(testPane->paneProps.width == 50);
        CHECK(testPane->paneProps.height == 25);
    }
}

TEST_CASE("Window_WindowView_resizeCallback") { BASIL_LOCK_TEST
    SECTION("Calls resize function on window") {
        WindowView window = WindowView();
        WindowView::resizeCallback(window.glfwWindow, 60, 30);

        CHECK(window.windowProps.width == 60);
        CHECK(window.windowProps.height == 30);
    }
}

TEST_CASE("Window_WindowView_Builder") { BASIL_LOCK_TEST
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

        CHECK(window->windowProps.title == title);
        CHECK(window->windowProps.width == width);
        CHECK(window->windowProps.height == height);
        CHECK(window->topPane == pane);
    }
}
