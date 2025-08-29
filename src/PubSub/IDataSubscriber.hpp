#pragma once

#include <any>

#include "DataMessage.hpp"

namespace basil {

/** @brief Interface for PubSub subscriber */
class IDataSubscriber {
 public:
    /** @brief Virtual method to receive DataMessage object */
    virtual void receiveData(const DataMessage& /* message */) {}
};

}   // namespace basil
