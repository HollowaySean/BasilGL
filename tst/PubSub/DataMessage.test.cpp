#include <catch.hpp>

#include "PubSub/DataMessage.hpp"

using basil::DataMessage;

TEST_CASE("PubSub_DataMessage_getData") {
    const std::string data = "data";
    DataMessage message = DataMessage(data);

    SECTION("Returns value if castable to type") {
        auto result = message.getData<std::string>();

        REQUIRE(result.has_value());
        CHECK(result.value() == data);
    }

    SECTION("Returns shared pointer if castable to pointer type") {
        std::shared_ptr<std::string> ptrData =
            std::make_shared<std::string>(data);
        message = DataMessage(ptrData);
        auto result = message.getData<std::string>();

        REQUIRE(result.has_value());
        CHECK(result.value() == data);
    }

    SECTION("Returns nullopts if payload cannot be cast to type") {
        auto result = message.getData<float>();

        REQUIRE_FALSE(result.has_value());
    }
}
