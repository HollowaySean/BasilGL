#include <sstream>

#include <catch.hpp>

#include <Basil/Logging.hpp>

using basil::Logger;
using basil::LogLevel;

TEST_CASE("Logging_Logger_log") {
    Logger& logger = Logger::get();

    std::string message = "message";

    SECTION("Sends message if level is higher than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.log(message, LogLevel::WARN);

        REQUIRE(logger.getLastLevel() == LogLevel::WARN);
        REQUIRE(logger.getLastOutput() == message);
        REQUIRE(logger.didOutputLastMessage());
    }

    SECTION("Does not send message if level is lower than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.log(message, LogLevel::DEBUG);

        REQUIRE(logger.getLastLevel() == LogLevel::DEBUG);
        REQUIRE(logger.getLastOutput() == message);
        REQUIRE_FALSE(logger.didOutputLastMessage());
    }
}

TEST_CASE("Logging_Logger_lineBreak") {
    Logger& logger = Logger::get();

    std::string message = "message";

    SECTION("Sends line break if level is higher than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.lineBreak(LogLevel::WARN);

        REQUIRE(logger.getLastLevel() == LogLevel::WARN);
        REQUIRE(logger.getLastOutput() == "\n");
        REQUIRE(logger.didOutputLastMessage());
    }

    SECTION("Does not send line break if level is lower than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.lineBreak(LogLevel::DEBUG);

        REQUIRE(logger.getLastLevel() == LogLevel::DEBUG);
        REQUIRE(logger.getLastOutput() == "\n");
        REQUIRE_FALSE(logger.didOutputLastMessage());
    }
}
