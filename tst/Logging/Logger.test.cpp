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

        CHECK(logger.getLastLevel() == LogLevel::WARN);
        CHECK(logger.getLastOutput() == message);
        CHECK(logger.didOutputLastMessage());
    }

    SECTION("Does not send message if level is lower than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.log(message, LogLevel::DEBUG);

        CHECK(logger.getLastLevel() == LogLevel::DEBUG);
        CHECK(logger.getLastOutput() == message);
        CHECK_FALSE(logger.didOutputLastMessage());
    }
}

TEST_CASE("Logging_Logger_lineBreak") {
    Logger& logger = Logger::get();

    std::string message = "message";

    SECTION("Sends line break if level is higher than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.lineBreak(LogLevel::WARN);

        CHECK(logger.getLastLevel() == LogLevel::WARN);
        CHECK(logger.getLastOutput() == "\n");
        CHECK(logger.didOutputLastMessage());
    }

    SECTION("Does not send line break if level is lower than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.lineBreak(LogLevel::DEBUG);

        CHECK(logger.getLastLevel() == LogLevel::DEBUG);
        CHECK(logger.getLastOutput() == "\n");
        CHECK_FALSE(logger.didOutputLastMessage());
    }
}
