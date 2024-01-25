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
        std::filesystem::path(SOURCE_DIR) / "Window/shaders/default.frag";


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
        .xOffset = 100,
        .yOffset = 0
    };

    firstPane = new GLTexturePane(paneProps, fragmentPath);
    firstPane->addTexture(texture);
    secondPane = new GLTexturePane(paneProps, fragmentPath);
    secondPane->addTexture(texture);

    topPane = new SplitPane(paneProps, PaneOrientation::VERTICAL);
    topPane->setFirstPane(firstPane);
    topPane->setSecondPane(secondPane);

    topPane->resizeToPercentage(25.f);
    topPane->setGapWidth(10);
}

void WindowView::onLoop() {
    // Dummy code
    // printf("In main loop\n");

    if (glfwWindowShouldClose(glfwWindow)) {
        currentState = State::REQUEST_STOP;

        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    // Clear background color
    glBlendFunc(GL_ONE, GL_ZERO);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void WindowView::initializeGLEWContext() {
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
}
