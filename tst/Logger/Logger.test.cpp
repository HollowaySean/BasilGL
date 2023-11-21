#include <sstream>

#include <catch.hpp>

#include "Logger.hpp"

TEST_CASE("Logger_Logger_Log") {
    std::ostringstream ostringstream;
    std::string message = "message";
    Logger& logger = Logger::get();

    SECTION("Sends message if level is higher than log level") {
        logger.Log(message, Level::WARN, ostringstream);

        REQUIRE(ostringstream.str() == message + '\n');
    }

    SECTION("Does not send message if level is lower than log level") {
        logger.Log(message, Level::DEBUG, ostringstream);

        REQUIRE(ostringstream.str() == "");
    }
}
