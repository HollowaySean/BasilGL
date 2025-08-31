#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if BASIL_INCLUDE_IMGUI
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#endif

#include <functional>
#include <map>
#include <memory>
#include <thread>

#include <Basil/Packages/Logging.hpp>

#include "Definitions.hpp"
#include "Window/IPane.hpp"

namespace basil {

/**
 * @brief Global container for GLEW & GLFW contexts
 *  using Singleton pattern.
 */
class BasilContext {
 public:
    /** @return Context object for a given thread. */
    static BasilContext& get(
        std::optional<std::thread::id> contextThreadID = std::nullopt
    );

    /** @brief  Creates context, if not defined. */
    void initialize();

    /** @brief  Destroys context, if defined. */
    void terminate();

    /** @return T/F if context has been initialized. */
    bool isInitialized() { return hasInitialized; }

    /** @return Pointer to GLFW window. */
    GLFWwindow* getGLFWWindow();

    /** @return ViewArea struct containing window size. */
    ViewArea getWindowArea();

    /** @brief Shorthand type for framebuffer callback */
    using BasilFrameBufferSizeFunc = std::function<void(int, int)>;

    /** @brief Add GLFW framebuffer size callback to list */
    unsigned int setGLFWFramebufferSizeCallback(
        const BasilFrameBufferSizeFunc& callback);

    /** @brief   Remove callback with ID
     *  @returns Boolean whether callback was found */
    bool removeGLFWFramebufferSizeCallback(unsigned int ID);

    /** @brief Shorthand type for mouse button callback */
    using BasilMouseButtonFunc = std::function<void(int, int, int)>;

    /** @brief Add GLFW mouse button callback to list */
    unsigned int setGLFWMouseButtonCallback(
        const BasilMouseButtonFunc& callback);

    /** @brief   Remove callback with ID
     *  @returns Boolean whether callback was found */
    bool removeGLFWMouseButtonCallback(unsigned int ID);

    /** @brief Shorthand type for keyboard callback */
    using BasilKeyFunc = std::function<void(int, int, int, int)>;

    /** @brief Add GLFW keyboard callback to list */
    unsigned int setGLFWKeyCallback(
        const BasilKeyFunc& callback);

    /** @brief   Remove callback with ID
     *  @returns Boolean whether callback was found */
    bool removeGLFWKeyCallback(unsigned int ID);

    /** @brief Shorthand type for cursor enter/exit callback */
    using BasilCursorEnterFunc = std::function<void(int)>;

    /** @brief Add GLFW cursor enter/exit callback to list */
    unsigned int setGLFWCursorEnterCallback(
        const BasilCursorEnterFunc& callback);

    /** @brief   Remove callback with ID
     *  @returns Boolean whether callback was found */
    bool removeGLFWCursorEnterCallback(unsigned int ID);

#ifdef TEST_BUILD

 public:
#else

 private:
#endif


#if BASIL_INCLUDE_IMGUI
    void initializeImGuiContext();
    void terminateImGuiContext();
#endif

    inline static std::map<std::thread::id, BasilContext> contexts
        = std::map<std::thread::id, BasilContext>();

    inline static const int GLFW_MAJOR_VERSION
        = BASIL_GLFW_VERSION_MAJOR;
    inline static const int GLFW_MINOR_VERSION
        = BASIL_GLFW_VERSION_MINOR;
    inline static const char* WINDOW_TITLE
        = BASIL_GLFW_UNINITIALIZED_WINDOW_TITLE;

    bool hasInitialized = false;

    GLFWwindow* glfwWindow = nullptr;

    std::map<unsigned int, BasilFrameBufferSizeFunc>
        framebufferCallbacks =
            std::map<unsigned int, BasilFrameBufferSizeFunc>();
    std::map<unsigned int, BasilMouseButtonFunc>
        mouseButtonCallbacks =
            std::map<unsigned int, BasilMouseButtonFunc>();
    std::map<unsigned int, BasilKeyFunc>
        keyCallbacks =
            std::map<unsigned int, BasilKeyFunc>();
    std::map<unsigned int, BasilCursorEnterFunc>
        cursorEnterCallbacks =
            std::map<unsigned int, BasilCursorEnterFunc>();

    unsigned int nextFrameBufferID = 0;
    unsigned int nextMouseButtonID = 0;
    unsigned int nextKeyID = 0;
    unsigned int nextCursorID = 0;

    void initializeGLFWContext();
    void initializeGLEWContext();

    void setGLFWCallbacks();
    static void onFrameBufferResize(
        GLFWwindow* window, int width, int height);
    static void onMouseButton(
        GLFWwindow* window, int button, int action, int mods);
    static void onKeyAction(
        GLFWwindow* window, int key, int scancode, int action, int mods);
    static void onCursorEnter(
        GLFWwindow* window, int entered);

    static void logGLFWError(GLenum errorCode);
    static void logGLFWWindowError(const GLFWwindow* window);
    static void logGLEWError(GLenum errorCode);

    static inline const Logger& logger = Logger::get();
};

}   // namespace basil
