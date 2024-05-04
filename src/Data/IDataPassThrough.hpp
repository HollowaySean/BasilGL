#ifndef SRC_DATA_IDATAPASSTHROUGH_HPP_
#define SRC_DATA_IDATAPASSTHROUGH_HPP_

#include <vector>

#include "IDataPublisher.hpp"
#include "IDataSubscriber.hpp"

namespace basil {

template<class T>
class IDataPassThrough : public IDataSubscriber<T>,
                         public IDataPublisher<T> {
 private:
    void receiveData(const T& dataModel) override {
        this->IDataPublisher<T>::publishData(dataModel);
    }
};

}  // namespace basil

#endif  // SRC_DATA_IDATAPASSTHROUGH_HPP_
