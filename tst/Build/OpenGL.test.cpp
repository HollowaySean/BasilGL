#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <catch.hpp>

TEST_CASE("Build_OpenGL_SmokeTest") {
    GLint success = glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_VISIBLE, false);
    GLFWwindow* newWindow = glfwCreateWindow(
        1, 1, "test",
        NULL, NULL);
    glfwMakeContextCurrent(newWindow);

    SECTION("Initializes GLFW") {
        CHECK(success == GLFW_TRUE);

        const char* errorMessage;
        glfwGetError(&errorMessage);
        INFO(errorMessage);
    }

    SECTION("Creates GLFW window") {
        CHECK(newWindow);
    }

    SECTION("Initializes GLEW") {
        GLenum error = glewInit();

        INFO(glewGetErrorString(error));
        CHECK(error == GLEW_OK);
        INFO(glewGetErrorString(error));
    }
}
