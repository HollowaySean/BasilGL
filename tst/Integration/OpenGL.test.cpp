#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <catch.hpp>

TEST_CASE("Intentionally breaking test, to test branch protection") {
    SECTION("Auto fail") {
        REQUIRE(false);
    }
}

TEST_CASE("Build_OpenGL_SmokeTest") {
    SECTION("Initializes GLEW and GLFW") {
        GLint success = glfwInit();

        const char* errorMessage;
        glfwGetError(&errorMessage);
        INFO(errorMessage);

        REQUIRE(success == GLFW_TRUE);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_VISIBLE, false);
        GLFWwindow* newWindow = glfwCreateWindow(
            1, 1, "test",
            NULL, NULL);

        glfwGetError(&errorMessage);
        INFO(errorMessage);

        REQUIRE(newWindow);

        glfwMakeContextCurrent(newWindow);
        GLenum error = glewInit();

        INFO(glewGetErrorString(error));
        REQUIRE(error == GLEW_OK);
    }
}
