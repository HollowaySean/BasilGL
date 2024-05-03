#ifndef SRC_DATA_IDATAPUBLISHER_HPP_
#define SRC_DATA_IDATAPUBLISHER_HPP_

#include <memory>
#include <set>
#include <unordered_map>
#include <utility>

#include "IDataSubscriber.hpp"

namespace basil {

// TODO(sholloway): Add revisioning system
class IDataPublisher {
 public:
    void publishData(const IDataModel& dataModel);

    void subscribe(
        std::shared_ptr<IDataSubscriber> subscriber,
        unsigned int modelID = 0);

    void unsubscribe(
        std::shared_ptr<IDataSubscriber> subscriber,
        unsigned int modelID = 0);

#ifndef TEST_BUILD

 private:
#endif
    struct Subscription {
     public:
        explicit Subscription(unsigned int modelID) {
            modelIDs = { modelID };
        }

        bool acceptsModel(unsigned int modelID) {
            return modelIDs.contains(0) || modelIDs.contains(modelID);
        }

        void addModel(unsigned int modelID) {
            modelIDs.emplace(modelID);
        }

        void removeModel(unsigned int modelID) {
            modelIDs.erase(modelID);
        }

#ifndef TEST_BUILD

     private:
#endif
        std::set<unsigned int> modelIDs;
    };

    std::unordered_map<
        std::shared_ptr<IDataSubscriber>, Subscription> subscriptions;
};

}  // namespace basil

#endif  // SRC_DATA_IDATAPUBLISHER_HPP_
