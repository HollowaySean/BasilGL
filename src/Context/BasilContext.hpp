#ifndef SRC_CONTEXT_BASILCONTEXT_HPP_
#define SRC_CONTEXT_BASILCONTEXT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <thread>
#include <memory>

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

    static GLFWwindow* getGLFWWindow();

    // Spinlocking for multithreaded testing support
    // TODO(sholloway): Clean this mess up
    inline static bool isLocked = false;
    inline static u_int64_t lockID = 0;

    inline static int spinTimeInMS = 100;
    inline static int timeoutInMS = 3000;

    static void lock(u_int64_t contextID) {
        spinIfLocked(contextID);

        isLocked = true;
        lockID = contextID;
    }

    static void unlock(u_int64_t contextID) {
        if (lockID != contextID) return;

        isLocked = false;
        lockID = 0;
    }

    static void spinIfLocked(u_int64_t contextID = 0) {
        int spinTime = 0;
        while (isLocked &&
                lockID != contextID &&
                spinTime < timeoutInMS) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(spinTimeInMS));
            spinTime += spinTimeInMS;
        }
    }

#ifdef TEST_BUILD

 public:
#else

 private:
#endif
    inline static const int BASIL_GLFW_VERSION_MAJOR = 4;
    inline static const int BASIL_GLFW_VERSION_MINOR = 5;
    inline static const char* WINDOW_TITLE =
        "Uninitialized BasilGL Window";

    inline static bool hasInitialized = false;

    GLFWwindow* glfwWindow = nullptr;

    void initializeGLFWContext();
    void initializeGLEWContext();

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
