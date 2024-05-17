#include <catch.hpp>

#include "PubSub/PubSubTestUtils.hpp"

using basil::DataMessage;
using basil::TestPublisher;
using basil::TestSubscriber;

TEST_CASE("PubSub_IDataPublisher_subscribe") {
    auto publisher = TestPublisher();
    auto subscriber = std::make_shared<TestSubscriber>();

    publisher.subscribe(subscriber);

    SECTION("Adds new subscriber to record of subscriptions") {
        CHECK(publisher.subscriptions.contains(subscriber));
        CHECK(publisher.subscriptions.size() == 1);
    }
}

TEST_CASE("PubSub_IDataPublisher_unsubscribe") {
    auto publisher = TestPublisher();
    auto subscriberOne = std::make_shared<TestSubscriber>();
    auto subscriberTwo = std::make_shared<TestSubscriber>();

    publisher.subscribe(subscriberOne);

    SECTION("Does nothing if subscriber is not subscribed") {
        CHECK(publisher.subscriptions.size() == 1);
        publisher.unsubscribe(subscriberTwo);
        CHECK(publisher.subscriptions.size() == 1);
    }

    SECTION("Removes subscription if subscribed") {
        CHECK(publisher.subscriptions.size() == 1);
        publisher.unsubscribe(subscriberOne);
        CHECK(publisher.subscriptions.size() == 0);
    }
}

TEST_CASE("PubSub_IDataPublisher_publishData") {
    auto publisher = TestPublisher();
    auto subscriber = std::make_shared<TestSubscriber>();

    auto message = DataMessage("Message content");

    SECTION("Sends DataMessage") {
        CHECK_FALSE(subscriber->hasReceivedData);

        publisher.subscribe(subscriber);
        publisher.publishData(message);

        CHECK(subscriber->hasReceivedData);
    }
}

TEST_CASE("PubSub_IDataPublisher_hasSubscriber") {
    auto publisher = TestPublisher();
    auto subscriber = std::make_shared<TestSubscriber>();

    SECTION("Returns true if subscriber exists") {
        CHECK_FALSE(publisher.hasSubscriber(subscriber));
        publisher.subscribe(subscriber);
        CHECK(publisher.hasSubscriber(subscriber));
    }
}

