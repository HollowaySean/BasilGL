#ifndef SRC_DATA_IDATASUBSCRIBER_HPP_
#define SRC_DATA_IDATASUBSCRIBER_HPP_

#include "DataMessage.hpp"

namespace basil {

/** @brief  Interface for receiver of PubSub data model */
class IDataSubscriber {
 private:
    virtual void receiveData(const DataMessage& dataMessage) = 0;

    friend class IDataPublisher;
};

}  // namespace basil

#endif  // SRC_DATA_IDATASUBSCRIBER_HPP_
