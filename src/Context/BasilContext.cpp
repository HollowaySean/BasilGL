#include <string>

#include "BasilContext.hpp"

namespace basil {

BasilContext& BasilContext::get(
    std::optional<std::thread::id> contextThreadID
) {
    std::thread::id threadID
        = contextThreadID.value_or(std::this_thread::get_id());

    if (contexts.count(threadID)) {
        return contexts.at(threadID);
    } else {
        contexts.emplace(threadID, BasilContext());
    }

    return contexts.at(threadID);
}

void BasilContext::initialize() {
    if (isInitialized()) return;

    hasInitialized = true;
    initializeGLFWContext();
    initializeGLEWContext();

    #if BASIL_INCLUDE_IMGUI
    initializeImGuiContext();
    #endif
}

void BasilContext::terminate() {
    if (!hasInitialized) return;

    #if BASIL_INCLUDE_IMGUI
    terminateImGuiContext();
    #endif

    glfwDestroyWindow(glfwWindow);
    hasInitialized = false;
    glfwWindow = nullptr;
}

void BasilContext::initializeGLFWContext() {
    // Initialize log errors, and update flag
    GLenum errorCode = glfwInit();
    logGLFWError(errorCode);
    hasInitialized &= static_cast<bool>(errorCode);

    // Create non-visible window and attach context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GLFW_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GLFW_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_VISIBLE, false);
    glfwWindow = glfwCreateWindow(
        1, 1, WINDOW_TITLE, NULL, NULL);
    glfwMakeContextCurrent(glfwWindow);

    // Save success/failure flag and log errors
    hasInitialized &= glfwWindow != nullptr;
    logGLFWWindowError(glfwWindow);

    if (!hasInitialized) {
        terminate();
    }

    // Set callback functions
    setGLFWCallbacks();
}

void BasilContext::initializeGLEWContext() {
    GLenum errorCode = glewInit();
    logGLEWError(errorCode);

    // Save success/failure flag
    hasInitialized &= !(errorCode);
}

void BasilContext::logGLFWError(GLenum errorCode) {
    Logger& logger = Logger::get();

    if (errorCode) {
        logger.log("GLFW context initialized successfully.", LogLevel::INFO);
    } else {
        const char* errorMessage;
        glfwGetError(&errorMessage);

        std::string errorMessageString = errorMessage ?
            std::string(errorMessage) : "Could not read error message.";

        logger.log("GLFW failed to initialize. Error: "
            + errorMessageString, LogLevel::ERROR);
    }
}

void BasilContext::logGLFWWindowError(const GLFWwindow* window) {
    Logger& logger = Logger::get();

    if (window) {
        logger.log("GLFW window created successfully.", LogLevel::INFO);
    } else {
        logger.log("GLFW failed to create window.", LogLevel::ERROR);
    }
}

void BasilContext::logGLEWError(GLenum errorCode) {
    Logger& logger = Logger::get();

    if (errorCode == GLEW_OK) {
        logger.log("GLEW context initialized successfully.", LogLevel::INFO);
    } else {
        const char* errorMessage =
            reinterpret_cast<const char*>(glewGetErrorString(errorCode));
        std::string errorMessageString = std::string(errorMessage);

        logger.log("GLEW failed to initialize. Error: "
            + errorMessageString, LogLevel::ERROR);
    }
}

GLFWwindow* BasilContext::getGLFWWindow() {
    if (!hasInitialized) {
        initialize();
    }

    return glfwWindow;
}

ViewArea BasilContext::getWindowArea() {
    if (glfwWindow == nullptr) {
        return { 0, 0, 0, 0 };
    }

    int width, height;
    glfwGetFramebufferSize(glfwWindow, &width, &height);

    return { width, height, 0, 0 };
}

void BasilContext::setGLFWCallbacks() {
    if (glfwWindow == nullptr) {
        return;
    }

    glfwSetFramebufferSizeCallback(glfwWindow,
        BasilContext::onFrameBufferResize);
    glfwSetMouseButtonCallback(glfwWindow,
        BasilContext::onMouseButton);
    glfwSetKeyCallback(glfwWindow,
        BasilContext::onKeyAction);
    glfwSetCursorEnterCallback(glfwWindow,
        BasilContext::onCursorEnter);
}

void BasilContext::onFrameBufferResize(
        GLFWwindow* /* window */, int width, int height) {
    const BasilContext& currentContext = get();
    for (const auto& callback : currentContext.framebufferCallbacks) {
        callback.second(width, height);
    }
}

void BasilContext::onMouseButton(
        GLFWwindow* /* window */, int button, int action, int mods) {
    const BasilContext& currentContext = get();
    for (const auto& callback : currentContext.mouseButtonCallbacks) {
        callback.second(button, action, mods);
    }
}

void BasilContext::onKeyAction(
        GLFWwindow* /* window */, int key, int scancode, int action, int mods) {
    const BasilContext& currentContext = get();
    for (const auto& callback : currentContext.keyCallbacks) {
        callback.second(key, scancode, action, mods);
    }
}

void BasilContext::onCursorEnter(
        GLFWwindow* /* window */, int entered) {
    const BasilContext& currentContext = get();
    for (const auto& callback : currentContext.cursorEnterCallbacks) {
        callback.second(entered);
    }
}

unsigned int BasilContext::setGLFWFramebufferSizeCallback(
        const BasilFrameBufferSizeFunc& callback) {
    framebufferCallbacks.emplace(nextFrameBufferID, callback);
    return nextFrameBufferID++;
}

bool BasilContext::removeGLFWFramebufferSizeCallback(unsigned int ID) {
    if (framebufferCallbacks.contains(ID)) {
        framebufferCallbacks.erase(ID);
        return true;
    }

    return false;
}

unsigned int BasilContext::setGLFWMouseButtonCallback(
        const BasilMouseButtonFunc& callback) {
    mouseButtonCallbacks.emplace(nextMouseButtonID, callback);
    return nextMouseButtonID++;
}

bool BasilContext::removeGLFWMouseButtonCallback(unsigned int ID) {
    if (mouseButtonCallbacks.contains(ID)) {
        mouseButtonCallbacks.erase(ID);
        return true;
    }

    return false;
}

unsigned int BasilContext::setGLFWKeyCallback(
        const BasilKeyFunc& callback) {
    keyCallbacks.emplace(nextKeyID, callback);
    return nextKeyID++;
}

bool BasilContext::removeGLFWKeyCallback(unsigned int ID) {
    if (keyCallbacks.contains(ID)) {
        keyCallbacks.erase(ID);
        return true;
    }

    return false;
}

unsigned int BasilContext::setGLFWCursorEnterCallback(
        const BasilCursorEnterFunc& callback) {
    cursorEnterCallbacks.emplace(nextCursorID, callback);
    return nextCursorID++;
}

bool BasilContext::removeGLFWCursorEnterCallback(unsigned int ID) {
    if (cursorEnterCallbacks.contains(ID)) {
        cursorEnterCallbacks.erase(ID);
        return true;
    }

    return false;
}

#if BASIL_INCLUDE_IMGUI

void BasilContext::initializeImGuiContext() {
        // Create ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Set default I/O configurations
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.IniFilename = NULL;
        io.LogFilename = NULL;

        // Attach to GLFW context
        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
        ImGui_ImplOpenGL3_Init();
}

void BasilContext::terminateImGuiContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

#endif  // BASIL_INCLUDE_IMGUI

}  // namespace basil
