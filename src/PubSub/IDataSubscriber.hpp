#ifndef SRC_PUBSUB_IDATASUBSCRIBER_HPP_
#define SRC_PUBSUB_IDATASUBSCRIBER_HPP_

#include <any>

#include "DataMessage.hpp"

namespace basil {

// TODO(sholloway): Documentation
class IDataSubscriber {
 public:
    virtual void receiveData(const DataMessage& message) {}
};

}  // namespace basil

#endif  // SRC_PUBSUB_IDATASUBSCRIBER_HPP_
