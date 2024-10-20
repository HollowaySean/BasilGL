#pragma once

#include "PubSub/DataMessage.hpp"
#include "PubSub/IDataPublisher.hpp"
#include "PubSub/IDataSubscriber.hpp"
#include "Data/ShaderUniformModel.hpp"

namespace basil {

class TestPublisher : public IDataPublisher {
 public:
    void publishData(const DataMessage& message) override {
        hasPublishedData = true;
        IDataPublisher::publishData(message);
    }

    bool hasPublishedData = false;
};

class TestSubscriber : public IDataSubscriber {
 public:
    void receiveData(const DataMessage& message) override {
        hasReceivedData = true;
        IDataSubscriber::receiveData(message);
    }

    bool hasReceivedData = false;
};

}   // namespace basil
