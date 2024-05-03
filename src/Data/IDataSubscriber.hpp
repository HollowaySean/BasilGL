#ifndef SRC_DATA_IDATASUBSCRIBER_HPP_
#define SRC_DATA_IDATASUBSCRIBER_HPP_

#include "IDataModel.hpp"

namespace basil {

template <class T>
requires std::is_base_of_v<IDataModel, T>
class IDataSubscriber {
 private:
    virtual void receiveData(const T& dataModel) = 0;

    template <class U>
    friend class IDataPublisher;
};

}  // namespace basil

#endif  // SRC_DATA_IDATASUBSCRIBER_HPP_
