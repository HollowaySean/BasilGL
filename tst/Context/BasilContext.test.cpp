#include <catch.hpp>

#include <Basil/Context.hpp>

using basil::BasilContextLock;
using basil::BasilContext;
using basil::Logger;
using basil::LogLevel;

TEST_CASE("Context_BasilContext_logGLFWError") {
    auto lock = BasilContextLock(std::hash<std::string>{}(
        Catch::getResultCapture().getCurrentTestName()));

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

TEST_CASE("Context_BasilContext_logGLFWWindowError") {
    auto lock = BasilContextLock(std::hash<std::string>{}(
        Catch::getResultCapture().getCurrentTestName()));

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

TEST_CASE("Context_BasilContext_logGLEWError") {
    auto lock = BasilContextLock(std::hash<std::string>{}(
        Catch::getResultCapture().getCurrentTestName()));

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

TEST_CASE("Context_BasilContext_lock") {
    SECTION("Sets lock ID and status") {
        u_int64_t lockID = 5;

        // Spin in case of other tests using lock
        BasilContext::spinIfLocked(lockID);

        BasilContext::lock(lockID);

        REQUIRE(BasilContext::lockID == lockID);
        REQUIRE(BasilContext::isLocked);

        BasilContext::unlock(lockID);
    }
}

TEST_CASE("Context_BasilContext_unlock") {
    SECTION("Does nothing if locked by another process") {
        u_int64_t lockID = 6;
        u_int64_t secondID = 7;

        // Spin in case of other tests using lock
        BasilContext::spinIfLocked(lockID);

        BasilContext::lock(lockID);

        REQUIRE(BasilContext::lockID == lockID);
        REQUIRE(BasilContext::isLocked);

        BasilContext::unlock(secondID);

        REQUIRE(BasilContext::lockID == lockID);
        REQUIRE(BasilContext::isLocked);
    }

    SECTION("Removes lock if set by same process") {
        u_int64_t lockID = 8;

        // Spin in case of other tests using lock
        BasilContext::spinIfLocked(lockID);

        BasilContext::lock(lockID);

        REQUIRE(BasilContext::lockID == lockID);
        REQUIRE(BasilContext::isLocked);

        BasilContext::unlock(lockID);

        REQUIRE(BasilContext::lockID == 0);
        REQUIRE_FALSE(BasilContext::isLocked);
    }
}

TEST_CASE("Context_BasilContext_spinIfLocked") {
    SECTION("Spins if locked by another process") {
        u_int64_t lockID = 9;
        u_int64_t secondID = 10;

        // Spin in case of other tests using lock
        BasilContext::spinIfLocked(lockID);

        BasilContext::lock(lockID);

        REQUIRE(BasilContext::lockID == lockID);
        REQUIRE(BasilContext::isLocked);

        auto start = std::chrono::system_clock::now();
        BasilContext::spinIfLocked(secondID);
        auto end = std::chrono::system_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        REQUIRE(duration.count() >= BasilContext::spinTimeInMS);
    }
}
