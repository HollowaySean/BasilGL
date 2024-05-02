#ifndef SRC_DATA_IDATASUBSCRIBER_HPP_
#define SRC_DATA_IDATASUBSCRIBER_HPP_

#include "DataModel.hpp"

namespace basil {

class IDataSubscriber {
 private:
    friend class IDataPublisher;
    virtual void receiveData(const DataModel& dataModel) = 0;
};

}

#endif  // SRC_DATA_IDATASUBSCRIBER_HPP_
