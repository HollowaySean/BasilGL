#include <stdio.h>

#include <chrono>
#include <thread>
#include <cmath>
#include <vector>

#include "WindowView.hpp"

WindowView::WindowView(): glfwWindow() {
    topPane = nullptr;
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

    std::filesystem::path fragmentPath =
        std::filesystem::path(SOURCE_DIR) / "Window/temp/test.frag";


    // NOTE TO SELF: ALL OF THESE OBJECTS DISAPPEAR IN LOOP

    testTexture = std::vector<float>();
    for (int i = 0; i < 100; i++) {
        testTexture.push_back(static_cast<float>(i) / 100.0f);
    }

    textureProps = new GLTextureProps();
    textureProps->name = "testTexture";
    textureProps->width = 10;
    textureProps->height = 10;
    textureProps->format = GL_RED;
    textureProps->internalFormat = GL_R32F;
    textureProps->dataType = GL_FLOAT;

    texture = new GLTexture<float>(testTexture, *textureProps);

    PaneProps paneProps = {
        .width = windowOptions.width,
        .height = windowOptions.height,
        .xOffset = 0,
        .yOffset = 0
    };

    firstPane = new GLTexturePane(paneProps, fragmentPath, *texture);
    secondPane = new GLTexturePane(paneProps, fragmentPath, *texture);

    topPane = new SplitPane(paneProps, PaneOrientation::VERTICAL);
    topPane->setFirstPane(firstPane);
    topPane->setSecondPane(secondPane);
}

void WindowView::onLoop() {
    // Dummy code
    // printf("In main loop\n");

    if (glfwWindowShouldClose(glfwWindow)) {
        currentState = State::REQUEST_STOP;

        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    topPane->draw();

    glfwSwapBuffers(glfwWindow);
    glfwPollEvents();
}

void WindowView::onStop() {
    // Dummy code
    logger.log("Stopping loop", Level::INFO);
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
