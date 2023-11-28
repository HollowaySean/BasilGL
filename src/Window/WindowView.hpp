#ifndef SRC_WINDOW_WINDOWVIEW_HPP_
#define SRC_WINDOW_WINDOWVIEW_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "IFrameProcess.hpp"
#include "GLTexturePane.hpp"

// #include "../Frame/IFrameProcess.hpp"

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

    void onStart() override;
    void onStop() override;

    /** @brief Main loop function for IFrameProcess parent class. */
    void onLoop() override;

 private:
    WindowOptions windowOptions;
    GLFWwindow* glfwWindow = nullptr;
    GLFWwindow* createGLFWWindow();
    void initializeGLFWContext();
    void initializeGLEWContext();

    std::vector<float>* testTexture;
    GLTexturePane *pane;

    Logger& logger = Logger::get();
};

#endif  // SRC_WINDOW_WINDOWVIEW_HPP_
