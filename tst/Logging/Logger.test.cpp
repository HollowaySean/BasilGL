#include <sstream>

#include <catch.hpp>

#include "Logger.hpp"

TEST_CASE("Logging_Logger_log") {
    std::ostringstream ostringstream;
    std::string message = "message";
    Logger& logger = Logger::get();

    SECTION("Sends message if level is higher than log level") {
        logger.setLevel(Level::INFO);
        logger.log(message, Level::WARN, ostringstream);

        REQUIRE(ostringstream.str() == "[WARN]: " + message + '\n');
    }

    SECTION("Does not send message if level is lower than log level") {
        logger.setLevel(Level::INFO);
        logger.log(message, Level::DEBUG, ostringstream);

        REQUIRE(ostringstream.str() == "");
    }
}

TEST_CASE("Logging_Logger_lineBreak") {
    std::ostringstream ostringstream;
    std::string message = "message";
    Logger& logger = Logger::get();

    SECTION("Sends line break if level is higher than log level") {
        logger.setLevel(Level::INFO);
        logger.lineBreak(Level::WARN, ostringstream);

        REQUIRE(ostringstream.str() == "\n");
    }

    SECTION("Does not send line break if level is lower than log level") {
        logger.setLevel(Level::INFO);
        logger.lineBreak(Level::DEBUG, ostringstream);

        REQUIRE(ostringstream.str() == "");
    }
}
