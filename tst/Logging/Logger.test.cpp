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
        REQUIRE(logger.getLastOutput() == "[WARN]: " + message + '\n');
    }

    SECTION("Does not send message if level is lower than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.log(message, LogLevel::DEBUG);

        REQUIRE(logger.getLastLevel() == LogLevel::DEBUG);
        REQUIRE(logger.getLastOutput() == "");
    }
}

TEST_CASE("Logging_Logger_lineBreak") {
    std::string message = "message";
    Logger& logger = Logger::get();

    SECTION("Sends line break if level is higher than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.lineBreak(LogLevel::WARN);

        REQUIRE(logger.getLastLevel() == LogLevel::WARN);
        REQUIRE(logger.getLastOutput() == "\n");
    }

    SECTION("Does not send line break if level is lower than log level") {
        logger.setLevel(LogLevel::INFO);
        logger.lineBreak(LogLevel::DEBUG);

        REQUIRE(logger.getLastLevel() == LogLevel::DEBUG);
        REQUIRE(logger.getLastOutput() == "");
    }
}
