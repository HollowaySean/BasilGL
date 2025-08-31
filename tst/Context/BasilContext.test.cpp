#include <catch.hpp>

#include "Context/BasilContext.hpp"
#include "OpenGL/GLTestUtils.hpp"

using basil::BasilContext;
using basil::Logger;
using basil::LogLevel;

TEST_CASE("Context_BasilContext_setGLFW[...]Callback") { BASIL_LOCK_TEST
    bool hasBeenCalled = false;
    auto callback =
        [&](int...){ hasBeenCalled = true; };

    BasilContext& context = BasilContext::get();

    SECTION("setGLFWFramebufferSizeCallback") {
        unsigned int ID =
            context.setGLFWFramebufferSizeCallback(callback);
        CHECK(context.framebufferCallbacks.contains(ID));

        CHECK_FALSE(hasBeenCalled);
        context.onFrameBufferResize(nullptr, 0, 0);
        CHECK(hasBeenCalled);

        context.removeGLFWFramebufferSizeCallback(ID);
        CHECK_FALSE(context.framebufferCallbacks.contains(ID));
    }

    SECTION("setGLFWMouseButtonCallback") {
        unsigned int ID =
            context.setGLFWMouseButtonCallback(callback);
        CHECK(context.mouseButtonCallbacks.contains(ID));

        CHECK_FALSE(hasBeenCalled);
        context.onMouseButton(nullptr, 0, 0, 0);
        CHECK(hasBeenCalled);

        context.removeGLFWMouseButtonCallback(ID);
        CHECK_FALSE(context.mouseButtonCallbacks.contains(ID));
    }

    SECTION("setGLFWKeyCallback") {
        unsigned int ID =
            context.setGLFWKeyCallback(callback);
        CHECK(context.keyCallbacks.contains(ID));

        context.setGLFWKeyCallback(callback);
        CHECK_FALSE(hasBeenCalled);
        context.onKeyAction(nullptr, 0, 0, 0, 0);
        CHECK(hasBeenCalled);

        context.removeGLFWKeyCallback(ID);
        CHECK_FALSE(context.keyCallbacks.contains(ID));
    }

    SECTION("setGLFWCursorEnterCallback") {
        unsigned int ID =
            context.setGLFWCursorEnterCallback(callback);
        CHECK(context.cursorEnterCallbacks.contains(ID));

        context.setGLFWCursorEnterCallback(callback);
        CHECK_FALSE(hasBeenCalled);
        context.onCursorEnter(nullptr, 0);
        CHECK(hasBeenCalled);

        context.removeGLFWCursorEnterCallback(ID);
        CHECK_FALSE(context.cursorEnterCallbacks.contains(ID));
    }
}

TEST_CASE("Context_BasilContext_logGLFWError") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();

    SECTION("Logs info on success") {
        BasilContext::logGLFWError(1);

        CHECK(logger.getLastOutput() ==
            "GLFW context initialized successfully.");
        CHECK(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error on failure") {
        BasilContext::logGLFWError(0);

        CHECK(logger.getLastLevel() == LogLevel::ERROR);
    }
}

TEST_CASE("Context_BasilContext_logGLFWWindowError") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();
    BasilContext& context = BasilContext::get();

    SECTION("Logs info on success") {
        GLFWwindow* window = context.getGLFWWindow();
        BasilContext::logGLFWWindowError(window);

        CHECK(logger.getLastOutput() ==
            "GLFW window created successfully.");
        CHECK(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error on failure") {
        context.initialize();
        CHECK(context.isInitialized());

        BasilContext::logGLFWWindowError(nullptr);

        CHECK(logger.getLastOutput() == "GLFW failed to create window.");
        CHECK(logger.getLastLevel() == LogLevel::ERROR);
    }
}

TEST_CASE("Context_BasilContext_logGLEWError") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();

    SECTION("Logs info on success") {
        BasilContext::logGLEWError(GLEW_OK);

        CHECK(logger.getLastOutput() ==
            "GLEW context initialized successfully.");
        CHECK(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error on failure") {
        BasilContext::logGLEWError(!GLEW_OK);

        CHECK(logger.getLastLevel() == LogLevel::ERROR);
    }
}
