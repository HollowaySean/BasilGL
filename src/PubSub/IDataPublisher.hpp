#pragma once

#include <memory>
#include <set>

#include "DataMessage.hpp"
#include "IDataSubscriber.hpp"

namespace basil {

/** @brief  Interface for publisher of PubSub data model */
class IDataPublisher {
 public:
    /** @brief Send data model to subscribers */
    virtual void publishData(const DataMessage& dataMessage) {
        for (auto subscriber : subscriptions) {
            subscriber->receiveData(dataMessage);
        }
    }

    /** @brief Add IDataSubscriber */
    void subscribe(std::shared_ptr<IDataSubscriber> subscriber) {
        subscriptions.emplace(subscriber);
    }


    /** @brief Remove IDataSubscriber */
    void unsubscribe(std::shared_ptr<IDataSubscriber> subscriber) {
        if (!subscriptions.contains(subscriber)) { return; }

        subscriptions.erase(subscriber);
    }

    /** @returns Boolean indicating whether IDataSubscriber is subscribed. */
    bool hasSubscriber(
            std::shared_ptr<IDataSubscriber> subscriber) {
        return subscriptions.contains(subscriber);
    }

    std::set<std::shared_ptr<IDataSubscriber>> subscriptions;
};

}   // namespace basil
