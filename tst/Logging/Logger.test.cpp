#include <sstream>

#include <catch.hpp>

#include <Basil/Logging.hpp>

TEST_CASE("Logging_Logger_log") {
    Logger& logger = Logger::get();
    std::string message = "message";

    SECTION("Sends message if level is higher than log level") {
        logger.setLevel(Level::INFO);
        logger.log(message, Level::WARN);

        REQUIRE(logger.getLastLevel() == Level::WARN);
        REQUIRE(logger.getLastOutput() == "[WARN]: " + message + '\n');
    }

    SECTION("Does not send message if level is lower than log level") {
        logger.setLevel(Level::INFO);
        logger.log(message, Level::DEBUG);

        REQUIRE(logger.getLastLevel() == Level::DEBUG);
        REQUIRE(logger.getLastOutput() == "");
    }
}

TEST_CASE("Logging_Logger_lineBreak") {
    std::string message = "message";
    Logger& logger = Logger::get();

    SECTION("Sends line break if level is higher than log level") {
        logger.setLevel(Level::INFO);
        logger.lineBreak(Level::WARN);

        REQUIRE(logger.getLastLevel() == Level::WARN);
        REQUIRE(logger.getLastOutput() == "\n");
    }

    SECTION("Does not send line break if level is lower than log level") {
        logger.setLevel(Level::INFO);
        logger.lineBreak(Level::DEBUG);

        REQUIRE(logger.getLastLevel() == Level::DEBUG);
        REQUIRE(logger.getLastOutput() == "");
    }
}
