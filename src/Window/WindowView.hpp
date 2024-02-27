#ifndef SRC_WINDOW_WINDOWVIEW_HPP_
#define SRC_WINDOW_WINDOWVIEW_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <Basil/Context.hpp>
#include <Basil/Frame.hpp>

#include "SplitPane.hpp"
#include "GLTexturePane.hpp"

namespace basil {

/**
 * @brief Struct holding basic properties of window.
*/
struct WindowProps {
 public:
    #ifndef TEST_BUILD
    inline static const int DEFAULT_WIDTH = 1080;
    inline static const int DEFAULT_HEIGHT = 920;
    #else
    inline static const int DEFAULT_WIDTH = 2;
    inline static const int DEFAULT_HEIGHT = 1;
    #endif

    inline static const std::string DEFAULT_TITLE = "Basil";

    std::string title = DEFAULT_TITLE;
    int width = DEFAULT_WIDTH;
    int height = DEFAULT_HEIGHT;
};

/**
 * @brief Outer window containing all UI elements and providing simple
 * public facade.
 */
class WindowView :  public IFrameProcess,
                    private BasilContextDependency {
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
    void setTopPane(std::shared_ptr<IPane> newTopPane);

    /** @returns PaneProps object for top pane. */
    PaneProps getTopPaneProps();

    /** @brief Update window size & title. */
    void setWindowProps(WindowProps newWindowProps);

    /** @returns Window settings as struct. */
    WindowProps getWindowProps () { return windowProps; }

#ifndef TEST_BUILD

 private:
#endif
    WindowProps windowProps;

    GLFWwindow* glfwWindow = nullptr;
    GLFWwindow* createGLFWWindow();

    void draw();
    void closeWindow();

    static void resizeCallback(GLFWwindow* window, int width, int height);
    void setCallbacks();
    void onResize(int width, int height);

    std::shared_ptr<IPane> topPane;

    BasilContext& context = BasilContext::get();
    Logger& logger = Logger::get();
};

}  // namespace basil

#endif  // SRC_WINDOW_WINDOWVIEW_HPP_
