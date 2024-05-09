#include <catch.hpp>

#include "PubSubTestUtils.hpp"

using basil::DataMessage;
using basil::TestDataModel;
using basil::TestPublisher;
using basil::TestSubscriber;

TEST_CASE("Data_IDataPublisher_subscribe") {
    auto publisher = TestPublisher();
    auto subscriber = std::make_shared<TestSubscriber>();

    publisher.subscribe(subscriber, 1);

    SECTION("Adds new subscriber to record of subscriptions") {
        CHECK(publisher.subscriptions.contains(subscriber));
        CHECK(publisher.subscriptions.size() == 1);

        auto subscription = publisher.subscriptions.at(subscriber);
        CHECK(subscription.modelIDs.contains(1));
    }

    publisher.subscribe(subscriber, 5);

    SECTION("Adds new model for existing subscription") {
        CHECK(publisher.subscriptions.size() == 1);

        auto subscription = publisher.subscriptions.at(subscriber);
        CHECK(subscription.modelIDs.contains(1));
        CHECK(subscription.modelIDs.contains(5));
    }
}

TEST_CASE("Data_IDataPublisher_unsubscribe") {
    auto publisher = TestPublisher();
    auto subscriberOne = std::make_shared<TestSubscriber>();
    auto subscriberTwo = std::make_shared<TestSubscriber>();

    publisher.subscribe(subscriberOne, 1);
    publisher.subscribe(subscriberOne, 2);

    SECTION("Does nothing if subscriber is not subscribed") {
        CHECK(publisher.subscriptions.size() == 1);
        publisher.unsubscribe(subscriberTwo, 0);
        CHECK(publisher.subscriptions.size() == 1);
    }

    SECTION("Removes model if provided modelID") {
        CHECK(publisher.subscriptions.size() == 1);
        publisher.unsubscribe(subscriberOne, 2);
        CHECK(publisher.subscriptions.size() == 1);

        auto subscription = publisher.subscriptions.at(subscriberOne);
        CHECK(subscription.modelIDs.contains(1));
        CHECK_FALSE(subscription.modelIDs.contains(2));
    }

    SECTION("Removes subscriber if modelID equals zero") {
        CHECK(publisher.subscriptions.size() == 1);
        publisher.unsubscribe(subscriberOne, 0);
        CHECK(publisher.subscriptions.size() == 0);
    }
}

TEST_CASE("Data_IDataPublisher_publishData") {
    auto publisher = TestPublisher();
    auto subscriber = std::make_shared<TestSubscriber>();
    auto model = TestDataModel();
    auto message = DataMessage(model);

    SECTION("Sends DataModel if subscriber accepts modelID") {
        CHECK_FALSE(subscriber->hasReceivedData);

        publisher.subscribe(subscriber,
            DataMessage::getIDForType<TestDataModel>());
        publisher.publishData(message);

        CHECK(subscriber->hasReceivedData);
    }

    SECTION("Sends DataModel if subscriber accepts all models") {
        CHECK_FALSE(subscriber->hasReceivedData);

        publisher.subscribe(subscriber);
        publisher.publishData(message);

        CHECK(subscriber->hasReceivedData);
    }

    SECTION("Does not send DataModel if not subscribed") {
        CHECK_FALSE(subscriber->hasReceivedData);

        publisher.subscribe(subscriber,
            DataMessage::getIDForType<TestDataModel>() + 1);
        publisher.publishData(message);

        CHECK_FALSE(subscriber->hasReceivedData);
    }
}

TEST_CASE("Data_IDataPublisher_hasSubscriber") {
    auto publisher = TestPublisher();
    auto subscriber = std::make_shared<TestSubscriber>();

    SECTION("Returns true if subscriber exists") {
        CHECK_FALSE(publisher.hasSubscriber(subscriber));
        publisher.subscribe(subscriber);
        CHECK(publisher.hasSubscriber(subscriber));
    }
}

