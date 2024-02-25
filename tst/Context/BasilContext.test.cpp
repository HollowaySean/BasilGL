#include <catch.hpp>

#include <Basil/Context.hpp>

using basil::BasilContext;
using basil::Logger;
using basil::LogLevel;

TEST_CASE("Context_BasilContext_logGLFWError") {
    Logger& logger = Logger::get();
    logger.clearTestInfo();

    SECTION("Logs info on success") {
        BasilContext::logGLFWError(1);

        REQUIRE(logger.getLastOutput() ==
            "[INFO]: GLFW context initialized successfully.\n");
        REQUIRE(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error on failure") {
        BasilContext::logGLFWError(0);

        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
    }
}

TEST_CASE("Context_BasilContext_logGLEWError") {
    Logger& logger = Logger::get();
    logger.clearTestInfo();

    SECTION("Logs info on success") {
        BasilContext::logGLEWError(GLEW_OK);

        REQUIRE(logger.getLastOutput() ==
            "[INFO]: GLEW context initialized successfully.\n");
        REQUIRE(logger.getLastLevel() ==
            LogLevel::INFO);
    }

    SECTION("Logs error on failure") {
        BasilContext::logGLEWError(!GLEW_OK);

        REQUIRE(logger.getLastLevel() == LogLevel::ERROR);
    }
}
