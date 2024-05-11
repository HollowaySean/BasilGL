#ifndef SRC_PUBSUB_IDATASUBSCRIBER_HPP_
#define SRC_PUBSUB_IDATASUBSCRIBER_HPP_

#include <any>

#include "DataMessage.hpp"

namespace basil {

class IDataSubscriber {
 public:
    virtual void receiveData(const DataMessage& message) = 0;
};

}  // namespace basil

#endif  // SRC_PUBSUB_IDATASUBSCRIBER_HPP_
