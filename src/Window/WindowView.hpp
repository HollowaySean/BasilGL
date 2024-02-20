#ifndef SRC_WINDOW_WINDOWVIEW_HPP_
#define SRC_WINDOW_WINDOWVIEW_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <string>
#include <vector>

#include <Basil/Frame.hpp>

#include "SplitPane.hpp"
#include "GLTexturePane.hpp"

/**
 * @brief Struct holding basic properties of window.
*/
struct WindowProps {
 public:
    inline static const int DEFAULT_WIDTH = 1080;
    inline static const int DEFAULT_HEIGHT = 920;
    inline static const std::string DEFAULT_TITLE = "Basil";

    std::string title = DEFAULT_TITLE;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
};

/**
 * @brief Outer window containing all UI elements and providing simple
 * public facade.
 */
class WindowView : public IFrameProcess {
 public:
    explicit WindowView(std::optional<WindowProps> windowProps = std::nullopt);
    ~WindowView();

    /** @brief Makes window visible. */
    void onStart() override;

    /** @brief Removes GLFW context and closes window. */
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
    WindowProps windowProps;

    GLFWwindow* glfwWindow = nullptr;
    GLFWwindow* createGLFWWindow();

    void initializeGLFWContext();
    void initializeGLEWContext();
    static void logGLFWError(GLenum errorCode);
    static void logGLEWError(GLenum errorCode);
    void draw();
    void closeWindow();

    static void resizeCallback(GLFWwindow* window, int width, int height);
    void setCallbacks();
    void onResize(int width, int height);

    IPane* topPane = nullptr;

    Logger& logger = Logger::get();
};

#endif  // SRC_WINDOW_WINDOWVIEW_HPP_
