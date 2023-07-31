#include <stdio.h>

#include <chrono>
#include <thread>

#include "WindowView.hpp"

WindowView::WindowView(): glfwWindow() {
    initializeGLFWContext();
}

WindowView::~WindowView() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void WindowView::onStart() {
    glfwWindow = createGLFWWindow();

    // GLEW can not be initialized before window context
    // has been created
    initializeGLEWContext();
    glViewport(0, 0,
        windowOptions.width,
        windowOptions.height);
}

void WindowView::onLoop() {
    // Dummy code
    // printf("In main loop\n");

    if (glfwWindowShouldClose(glfwWindow)) {
        currentState = State::REQUEST_STOP;

        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POLYGON);
        glVertex3f(0.5, 0.0, 0.5);
        glVertex3f(0.5, 0.0, 0.0);
        glVertex3f(0.0, 0.5, 0.0);
        glVertex3f(0.0, 0.0, 0.5);
    glEnd();

    glFlush();

    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

void WindowView::onStop() {
    // Dummy code
    printf("Stopping loop\n");
}

GLFWwindow* WindowView::createGLFWWindow() {
    GLFWwindow* newWindow = glfwCreateWindow(
        windowOptions.width,
        windowOptions.height,
        windowOptions.title.c_str(),
        NULL, NULL);

    if (!newWindow) {
        fprintf(stderr, "GLFW failed to create window.");
        glfwTerminate();
    }
    glfwMakeContextCurrent(newWindow);

    return newWindow;
}

void WindowView::initializeGLFWContext() {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW failed to initialize.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void WindowView::initializeGLEWContext() {
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
}
