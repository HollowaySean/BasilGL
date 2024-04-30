#include <catch.hpp>

#include <Basil/Context.hpp>

#include "Window/GLTestUtils.hpp"

using basil::BasilContextLock;
using basil::BasilContext;
using basil::Logger;
using basil::LogLevel;

TEST_CASE("Context_BasilContext_logGLFWError") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();

    SECTION("Logs info on success") {
        BasilContext::logGLFWError(1);

        REQUIRE(logger.getLastOutput() ==
            "GLFW context initialized successfully.");
        REQUIRE(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error on failure") {
        BasilContext::logGLFWError(0);

        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
    }
}

TEST_CASE("Context_BasilContext_logGLFWWindowError") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();

    SECTION("Logs info on success") {
        GLFWwindow* window = BasilContext::getGLFWWindow();
        BasilContext::logGLFWWindowError(window);

        REQUIRE(logger.getLastOutput() ==
            "GLFW window created successfully.");
        REQUIRE(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error and terminates on failure") {
        BasilContext::initialize();
        REQUIRE(BasilContext::isInitialized());

        BasilContext::logGLFWWindowError(nullptr);

        REQUIRE(logger.getLastOutput() == "GLFW failed to create window.");
        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
        REQUIRE_FALSE(BasilContext::isInitialized());

        // Re-initialize to prevent issues
        BasilContext::initialize();
    }
}

TEST_CASE("Context_BasilContext_logGLEWError") { BASIL_LOCK_TEST
    Logger& logger = Logger::get();

    SECTION("Logs info on success") {
        BasilContext::logGLEWError(GLEW_OK);

        REQUIRE(logger.getLastOutput() ==
            "GLEW context initialized successfully.");
        REQUIRE(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error on failure") {
        BasilContext::logGLEWError(!GLEW_OK);

        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
    }
}
