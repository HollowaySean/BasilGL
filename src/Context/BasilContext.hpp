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

#include <Basil/Packages/Logging.hpp>

#include "Definitions.hpp"

namespace basil {

/**
 * @brief Global container for GLEW & GLFW contexts
 *  using Singleton pattern.
 */
class BasilContext {
 public:
    /** @brief  Creates context, if not defined. */
    static void initialize();

    /** @brief  Destroys context, if defined. */
    static void terminate();

    /** @return T/F if context has been initialized. */
    static bool isInitialized() { return hasInitialized; }

    /** @return Instance of Singleton context. */
    static BasilContext& get() {
        static BasilContext instance;
        return instance;
    }

    /** @return Pointer to GLFW window. */
    static GLFWwindow* getGLFWWindow();

    /** @brief Shorthand type for framebuffer callback */
    using BasilFrameBufferSizeFunc = std::function<void(int, int)>;

    /** @brief Add GLFW framebuffer size callback to list */
    static unsigned int setGLFWFramebufferSizeCallback(
        const BasilFrameBufferSizeFunc& callback);

    /** @brief   Remove callback with ID
     *  @returns Boolean whether callback was found */
    static bool removeGLFWFramebufferSizeCallback(unsigned int ID);

    /** @brief Shorthand type for mouse button callback */
    using BasilMouseButtonFunc = std::function<void(int, int, int)>;

    /** @brief Add GLFW mouse button callback to list */
    static unsigned int setGLFWMouseButtonCallback(
        const BasilMouseButtonFunc& callback);

    /** @brief   Remove callback with ID
     *  @returns Boolean whether callback was found */
    static bool removeGLFWMouseButtonCallback(unsigned int ID);

    /** @brief Shorthand type for keyboard callback */
    using BasilKeyFunc = std::function<void(int, int, int, int)>;

    /** @brief Add GLFW keyboard callback to list */
    static unsigned int setGLFWKeyCallback(
        const BasilKeyFunc& callback);

    /** @brief   Remove callback with ID
     *  @returns Boolean whether callback was found */
    static bool removeGLFWKeyCallback(unsigned int ID);

    /** @brief Shorthand type for cursor enter/exit callback */
    using BasilCursorEnterFunc = std::function<void(int)>;

    /** @brief Add GLFW cursor enter/exit callback to list */
    static unsigned int setGLFWCursorEnterCallback(
        const BasilCursorEnterFunc& callback);

    /** @brief   Remove callback with ID
     *  @returns Boolean whether callback was found */
    static bool removeGLFWCursorEnterCallback(unsigned int ID);

#ifdef TEST_BUILD

 public:
#else

 private:
#endif


#if BASIL_INCLUDE_IMGUI
    static void initializeImGuiContext();
    static void terminateImGuiContext();
#endif

    inline static const int GLFW_MAJOR_VERSION
        = BASIL_GLFW_VERSION_MAJOR;
    inline static const int GLFW_MINOR_VERSION
        = BASIL_GLFW_VERSION_MINOR;
    inline static const char* WINDOW_TITLE
        = BASIL_GLFW_UNINITIALIZED_WINDOW_TITLE;

    inline static bool hasInitialized = false;

    GLFWwindow* glfwWindow = nullptr;

    static inline std::map<unsigned int, BasilFrameBufferSizeFunc>
        framebufferCallbacks =
            std::map<unsigned int, BasilFrameBufferSizeFunc>();
    static inline std::map<unsigned int, BasilMouseButtonFunc>
        mouseButtonCallbacks =
            std::map<unsigned int, BasilMouseButtonFunc>();
    static inline std::map<unsigned int, BasilKeyFunc>
        keyCallbacks =
            std::map<unsigned int, BasilKeyFunc>();
    static inline std::map<unsigned int, BasilCursorEnterFunc>
        cursorEnterCallbacks =
            std::map<unsigned int, BasilCursorEnterFunc>();

    static inline unsigned int nextFrameBufferID = 0;
    static inline unsigned int nextMouseButtonID = 0;
    static inline unsigned int nextKeyID = 0;
    static inline unsigned int nextCursorID = 0;

    void initializeGLFWContext();
    void initializeGLEWContext();

    static void setGLFWCallbacks();
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

    Logger& logger = Logger::get();

    // Inaccessable methods to enforce Singleton
    BasilContext() {}
    BasilContext(BasilContext const&);
    void operator=(BasilContext const&);
};

}   // namespace basil
