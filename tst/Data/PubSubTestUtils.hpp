#ifndef TST_DATA_PUBSUBTESTUTILS_HPP_
#define TST_DATA_PUBSUBTESTUTILS_HPP_

#include "Data/IDataModel.hpp"
#include "Data/IDataPublisher.hpp"
#include "Data/IDataSubscriber.hpp"
#include "Data/ShaderUniformModel.hpp"

namespace basil {

class TestDataModel : public IDataModel {};

class TestPublisher : public IDataPublisher<TestDataModel> {
 public:
    void publishData(const TestDataModel& dataModel) override {
        hasPublishedData = true;
        IDataPublisher::publishData(dataModel);
    }

    bool hasPublishedData = false;
};

class TestSubscriber : public IDataSubscriber<TestDataModel> {
 public:
    void receiveData(const TestDataModel& dataModel) override {
        hasReceivedData = true;
    }

    bool hasReceivedData = false;
};

class TestUniformSubscriber : public IDataSubscriber<ShaderUniformModel> {
 public:
    void receiveData(const ShaderUniformModel& dataModel) override {
        hasReceivedData = true;
    }

    bool hasReceivedData = false;
};

class TestUniformPublisher : public IDataPublisher<ShaderUniformModel> {
 public:
    void publishData(const ShaderUniformModel& dataModel) override {
        hasPublishedData = true;
        IDataPublisher::publishData(dataModel);
    }

    bool hasPublishedData = false;
};

}  // namespace basil

#endif  // TST_DATA_PUBSUBTESTUTILS_HPP_
