#ifndef TST_DATA_PUBSUBTESTUTILS_HPP_
#define TST_DATA_PUBSUBTESTUTILS_HPP_

#include "Data/IDataModel.hpp"
#include "Data/IDataPublisher.hpp"
#include "Data/IDataSubscriber.hpp"

namespace basil {

class TestPublisher : public IDataPublisher {};

class TestSubscriber : public IDataSubscriber {
 public:
    void receiveData(const IDataModel& dataModel) override {
        hasReceivedData = true;
    }

    bool hasReceivedData = false;
};

class TestDataModel : public IDataModel {};

}  // namespace basil

#endif  // TST_DATA_PUBSUBTESTUTILS_HPP_
