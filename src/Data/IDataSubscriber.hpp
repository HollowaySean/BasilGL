#ifndef SRC_DATA_IDATASUBSCRIBER_HPP_
#define SRC_DATA_IDATASUBSCRIBER_HPP_

#include "IDataModel.hpp"

namespace basil {

class IDataSubscriber {
 private:
    friend class IDataPublisher;
    virtual void receiveData(const IDataModel& dataModel) = 0;
};

}

#endif  // SRC_DATA_IDATASUBSCRIBER_HPP_
