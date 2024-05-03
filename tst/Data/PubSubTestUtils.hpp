#ifndef TST_DATA_PUBSUBTESTUTILS_HPP_
#define TST_DATA_PUBSUBTESTUTILS_HPP_

#include "Data/IDataModel.hpp"
#include "Data/IDataPublisher.hpp"
#include "Data/IDataSubscriber.hpp"

namespace basil {

class TestDataModel : public IDataModel {};

class TestPublisher : public IDataPublisher<TestDataModel> {};

class TestSubscriber : public IDataSubscriber<TestDataModel> {
 public:
    void receiveData(const TestDataModel& dataModel) override {
        hasReceivedData = true;
    }

    bool hasReceivedData = false;
};

}  // namespace basil

#endif  // TST_DATA_PUBSUBTESTUTILS_HPP_
