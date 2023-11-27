#ifndef TST_WINDOW_GLTESTUTILS_HPP_
#define TST_WINDOW_GLTESTUTILS_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct GLTestUtils {
    static void initializeGLContext() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, false);
        GLFWwindow* newWindow = glfwCreateWindow(
            1, 1, "test",
            NULL, NULL);
        glfwMakeContextCurrent(newWindow);
        glewInit();
    }

    static void deinitialize() {
        glfwTerminate();
    }
};

#endif  // TST_WINDOW_GLTESTUTILS_HPP_
