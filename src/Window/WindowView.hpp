#ifndef SRC_WINDOW_WINDOWVIEW_HPP_
#define SRC_WINDOW_WINDOWVIEW_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include <Basil/Frame.hpp>

#include "SplitPane.hpp"
#include "GLTexturePane.hpp"

// Hoisting
struct WindowOptions {
 public:
    std::string title = "My Window";
    int width = 1080;
    int height = 920;
};

/**
 * @brief Outer window containing all UI elements and providing simple
 * public facade.
 * TODO: Rewrite this documentation
 */
class WindowView : public IFrameProcess {
 public:
    WindowView();
    ~WindowView();

    void onStop() override;

    /** @brief Main loop function for IFrameProcess parent class. */
    void onLoop() override;

    /** @brief Sets top-level pane for window. */
    void setTopPane(IPane* newTopPane);

    /** @returns PaneProps object for top pane. */
    PaneProps getTopPaneProps();

#ifndef TEST_BUILD

 private:
#endif
    WindowOptions windowOptions;

    GLFWwindow* glfwWindow = nullptr;
    GLFWwindow* createGLFWWindow();

    void initializeGLFWContext();
    void initializeGLEWContext();
    void draw();
    void closeWindow();

    void setCallbacks();
    void onResize(GLFWwindow* window, int width, int height);

    IPane* topPane = nullptr;

    Logger& logger = Logger::get();
};

#endif  // SRC_WINDOW_WINDOWVIEW_HPP_
