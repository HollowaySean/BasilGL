#ifndef SRC_DATA_IDATAPUBLISHER_HPP_
#define SRC_DATA_IDATAPUBLISHER_HPP_

#include <memory>
#include <set>

#include "IDataSubscriber.hpp"

namespace basil {

class IDataPublisher {
 public:
    void publishData(const DataModel& dataModel) {
        for (auto subscriber : subscribers) {
            subscriber->receiveData(dataModel);
        }
    }

    void subscribe(std::shared_ptr<IDataSubscriber> subscriber) {
        subscribers.insert(subscriber);
    }

    void unsubscribe(std::shared_ptr<IDataSubscriber> subscriber) {
        subscribers.erase(subscriber);
    }

 private:
    std::set<std::shared_ptr<IDataSubscriber>> subscribers;
};

}  // namespace basil

#endif  // SRC_DATA_IDATAPUBLISHER_HPP_
