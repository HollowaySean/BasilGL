#include <catch.hpp>

#include "PubSub/IDataPassThrough.hpp"

#include "PubSubTestUtils.hpp"

using basil::DataMessage;
using basil::IDataPassThrough;
using basil::TestPublisher;
using basil::TestSubscriber;

TEST_CASE("PubSub_IDataPassThrough_receiveData") {
    auto subscriber = std::make_shared<TestSubscriber>();
    auto publisher = std::make_shared<TestPublisher>();

    auto message = DataMessage("Message content");

    auto passThrough = std::make_shared<IDataPassThrough>();
    publisher->subscribe(passThrough);
    passThrough->subscribe(subscriber);

    SECTION("Passes data on to subscribers") {
        CHECK_FALSE(subscriber->hasReceivedData);

        publisher->publishData(message);
        CHECK(subscriber->hasReceivedData);
    }
}
