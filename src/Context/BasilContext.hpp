#ifndef SRC_CONTEXT_BASILCONTEXT_HPP_
#define SRC_CONTEXT_BASILCONTEXT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>
#include <memory>
#include <vector>

#include "Definitions.hpp"
#include <Basil/Logging.hpp>

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

    using BasilFrameBufferSizeFunc = std::function<void(int, int)>;
    static void setGLFWFramebufferSizeCallback(
        const BasilFrameBufferSizeFunc& callback);

    using BasilMouseButtonFunc = std::function<void(int, int, int)>;
    static void setGLFWMouseButtonCallback(
        const BasilMouseButtonFunc& callback);

    using BasilKeyFunc = std::function<void(int, int, int, int)>;
    static void setGLFWKeyCallback(
        const BasilKeyFunc& callback);

    using BasilCursorEnterFunc = std::function<void(int)>;
    static void setGLFWCursorEnterCallback(
        const BasilCursorEnterFunc& callback);

#ifdef TEST_BUILD

 public:
#else

 private:
#endif

    inline static const int GLFW_MAJOR_VERSION
        = BASIL_GLFW_VERSION_MAJOR;
    inline static const int GLFW_MINOR_VERSION
        = BASIL_GLFW_VERSION_MINOR;
    inline static const char* WINDOW_TITLE
        = BASIL_GLFW_UNINITIALIZED_WINDOW_TITLE;

    inline static bool hasInitialized = false;

    GLFWwindow* glfwWindow = nullptr;

    static inline std::vector<BasilFrameBufferSizeFunc>
        framebufferCallbacks = std::vector<BasilFrameBufferSizeFunc>();
    static inline std::vector<BasilMouseButtonFunc>
        mouseButtonCallbacks = std::vector<BasilMouseButtonFunc>();
    static inline std::vector<BasilKeyFunc>
        keyCallbacks = std::vector<BasilKeyFunc>();
    static inline std::vector<BasilCursorEnterFunc>
        cursorEnterCallbacks = std::vector<BasilCursorEnterFunc>();

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

}  // namespace basil

#endif  // SRC_CONTEXT_BASILCONTEXT_HPP_
