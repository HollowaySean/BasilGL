#include <catch.hpp>

#include "PubSub/DataMessage.hpp"

using basil::DataMessage;

TEST_CASE("PubSub_DataMessage_getData") {
    const std::string data = "data";
    DataMessage message = DataMessage(data);

    SECTION("Returns value if type is correct") {
        auto result = message.getData<std::string>();

        REQUIRE(result.has_value());
        CHECK(result.value() == data);
    }

    SECTION("Returns nullopts if type is incorrect") {
        auto result = message.getData<float>();

        REQUIRE_FALSE(result.has_value());
    }
}
