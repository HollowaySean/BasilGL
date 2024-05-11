#ifndef SRC_DATA_IDATAPASSTHROUGH_HPP_
#define SRC_DATA_IDATAPASSTHROUGH_HPP_

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

}  // namespace basil

#endif  // SRC_DATA_IDATAPASSTHROUGH_HPP_
