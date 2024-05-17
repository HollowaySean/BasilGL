#include <catch.hpp>

#include "Context/BasilContext.hpp"
#include "Context/BasilContextLock.hpp"
#include "OpenGL/GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::BasilContext;
using basil::Logger;
using basil::LogLevel;

TEST_CASE("Context_BasilContext_setGLFW[...]Callback") { BASIL_LOCK_TEST
    bool hasBeenCalled = false;
    auto callback =
        [&](int...){ hasBeenCalled = true; };

    SECTION("setGLFWFramebufferSizeCallback") {
        BasilContext::setGLFWFramebufferSizeCallback(callback);
        CHECK_FALSE(hasBeenCalled);
        BasilContext::onFrameBufferResize(nullptr, 0, 0);
        CHECK(hasBeenCalled);
    }

    SECTION("setGLFWMouseButtonCallback") {
        BasilContext::setGLFWMouseButtonCallback(callback);
        CHECK_FALSE(hasBeenCalled);
        BasilContext::onMouseButton(nullptr, 0, 0, 0);
        CHECK(hasBeenCalled);
    }

    SECTION("setGLFWKeyCallback") {
        BasilContext::setGLFWKeyCallback(callback);
        CHECK_FALSE(hasBeenCalled);
        BasilContext::onKeyAction(nullptr, 0, 0, 0, 0);
        CHECK(hasBeenCalled);
    }

    SECTION("setGLFWCursorEnterCallback") {
        BasilContext::setGLFWCursorEnterCallback(callback);
        CHECK_FALSE(hasBeenCalled);
        BasilContext::onCursorEnter(nullptr, 0);
        CHECK(hasBeenCalled);
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

    SECTION("Logs info on success") {
        GLFWwindow* window = BasilContext::getGLFWWindow();
        BasilContext::logGLFWWindowError(window);

        CHECK(logger.getLastOutput() ==
            "GLFW window created successfully.");
        CHECK(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error and terminates on failure") {
        BasilContext::initialize();
        CHECK(BasilContext::isInitialized());

        BasilContext::logGLFWWindowError(nullptr);

        CHECK(logger.getLastOutput() == "GLFW failed to create window.");
        CHECK(logger.getLastLevel() == LogLevel::ERROR);
        CHECK_FALSE(BasilContext::isInitialized());

        // Re-initialize to prevent issues
        BasilContext::initialize();
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
