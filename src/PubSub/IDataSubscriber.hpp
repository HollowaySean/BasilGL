#ifndef SRC_PUBSUB_IDATASUBSCRIBER_HPP_
#define SRC_PUBSUB_IDATASUBSCRIBER_HPP_

#include <any>

#include "DataMessage.hpp"

namespace basil {

/** @brief Interface for PubSub subscriber */
class IDataSubscriber {
 public:
    /** @brief Virtual method to receive DataMessage object */
    virtual void receiveData(const DataMessage& message) {}
};

}  // namespace basil

#endif  // SRC_PUBSUB_IDATASUBSCRIBER_HPP_
