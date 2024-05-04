#include <catch.hpp>

#include "Data/IDataPassThrough.hpp"

#include "PubSubTestUtils.hpp"

using basil::IDataPassThrough;
using basil::TestDataModel;
using basil::TestPublisher;
using basil::TestSubscriber;

TEST_CASE("Data_IDataPassThrough_receiveData") {
    auto model = TestDataModel();
    auto subscriber = std::make_shared<TestSubscriber>();
    auto publisher = std::make_shared<TestPublisher>();

    auto passThrough = std::make_shared<IDataPassThrough<TestDataModel>>();
    publisher->subscribe(passThrough);
    passThrough->subscribe(subscriber);

    SECTION("Passes data on to subscribers") {
        CHECK_FALSE(subscriber->hasReceivedData);

        publisher->publishData(model);
        CHECK(subscriber->hasReceivedData);
    }
}
