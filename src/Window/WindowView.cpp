#include <stdio.h>

#include <chrono>
#include <thread>
#include <cmath>
#include <vector>

#include "WindowView.hpp"

WindowView::WindowView(): glfwWindow() {
    pane = nullptr;
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



    testTexture = new std::vector<float>();
    for (int i = 0; i < 100; i++) {
        testTexture->push_back(1.0f);
    }

    std::filesystem::path vertexPath = "/home/sholloway/dev/BlackHoleViz_v3/src/Window/temp/test.vert";
    std::filesystem::path fragmentPath = "/home/sholloway/dev/BlackHoleViz_v3/src/Window/temp/test.frag";

    GLVertexShader vertexShader = GLVertexShader(vertexPath);
    GLFragmentShader fragmentShader = GLFragmentShader(fragmentPath);
    GLShaderProgram shaderProgram = GLShaderProgram(vertexShader, fragmentShader);

    shaderProgram.setUniformInt(windowOptions.width, "u_width");
    shaderProgram.setUniformInt(windowOptions.height, "u_height");

    pane = new GLTexturePane(shaderProgram);
    pane->setTexture(testTexture);
    pane->setup();
}

void WindowView::onLoop() {
    // Dummy code
    // printf("In main loop\n");

    if (glfwWindowShouldClose(glfwWindow)) {
        currentState = State::REQUEST_STOP;

        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    float time = std::chrono::steady_clock::now().time_since_epoch().count();

    // glClearColor(
    //     0.5f + 0.5f*sin(time / (1000 * 1000 * 1000)),
    //     0.1f,
    //     0.5f + 0.5f*cos(time / (1000 * 1000 * 1000)),
    //     1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    pane->draw();

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
