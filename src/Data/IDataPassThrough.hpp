#ifndef SRC_DATA_IDATAPASSTHROUGH_HPP_
#define SRC_DATA_IDATAPASSTHROUGH_HPP_

#include <vector>

#include "IDataPublisher.hpp"
#include "IDataSubscriber.hpp"

namespace basil {

class IDataPassThrough : public IDataSubscriber,
                         public IDataPublisher {
 private:
    void receiveData(const IDataModel& dataModel) override {
        publishData(dataModel);
    }
};

}  // namespace basil

#endif  // SRC_DATA_IDATAPASSTHROUGH_HPP_
