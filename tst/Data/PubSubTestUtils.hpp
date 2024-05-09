#ifndef TST_DATA_PUBSUBTESTUTILS_HPP_
#define TST_DATA_PUBSUBTESTUTILS_HPP_

#include "Data/DataMessage.hpp"
#include "Data/IDataPublisher.hpp"
#include "Data/IDataSubscriber.hpp"
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
    }

    bool hasReceivedData = false;
};

}  // namespace basil

#endif  // TST_DATA_PUBSUBTESTUTILS_HPP_
