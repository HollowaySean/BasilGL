#pragma once

#include <vector>

#include "IDataPublisher.hpp"
#include "IDataSubscriber.hpp"

namespace basil {

/** @brief Interface which acts as publisher,
 *  and passes received DataModel on to subscribers. */
class IDataPassThrough : public IDataSubscriber,
                         public IDataPublisher {
 private:
    void receiveData(const DataMessage& message) override {
        this->IDataPublisher::publishData(message);
    }
};

}   // namespace basil
