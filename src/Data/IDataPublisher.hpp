#ifndef SRC_DATA_IDATAPUBLISHER_HPP_
#define SRC_DATA_IDATAPUBLISHER_HPP_

#include <memory>
#include <set>
#include <unordered_map>
#include <utility>

#include "IDataSubscriber.hpp"

namespace basil {

/** @brief  Interface for publisher of PubSub data model
 *  @tparam IDataModel implementation which is sent */
template <class T>
requires std::is_base_of_v<IDataModel, T>
class IDataPublisher {
 public:
    /** @brief Send data model to subscribers */
    virtual void publishData(const T& dataModel) {
        for (auto pair : subscriptions) {
            auto [ subscriber, subscription] = pair;

            if (subscription.acceptsModel(dataModel.getID())) {
                subscriber->receiveData(dataModel);
            }
        }
    }

    /** @brief Add subscriber
     *  @param subscriber   New IDataSubscriber
     *  @param modelID      ID of model to receive
     *  If modelID is 0, all models are subscribed to.
     */
    void subscribe(
            std::shared_ptr<IDataSubscriber<T>> subscriber,
            unsigned int modelID = 0) {
        if (subscriptions.contains(subscriber)) {
            subscriptions.at(subscriber).addModel(modelID);
        } else {
            subscriptions.emplace(subscriber, Subscription(modelID));
        }
    }


    /** @brief Remove model or subscriber
     *  @param subscriber   IDataSubscriber to remove
     *  @param modelID      ID of model to receive
     *  If modelID is 0, all models are unsubscribed from.
     */
    void unsubscribe(
            std::shared_ptr<IDataSubscriber<T>> subscriber,
            unsigned int modelID = 0) {
        if (!subscriptions.contains(subscriber)) { return; }

        if (modelID == 0) {
            subscriptions.erase(subscriber);
        } else {
            subscriptions.at(subscriber).removeModel(modelID);
        }
    }

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
        std::shared_ptr<IDataSubscriber<T>>, Subscription> subscriptions;
};

}  // namespace basil

#endif  // SRC_DATA_IDATAPUBLISHER_HPP_
