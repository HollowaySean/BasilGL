#include "IDataPublisher.hpp"

namespace basil {

void IDataPublisher::publishData(const IDataModel& dataModel) {
    for (auto pair : subscriptions) {
        auto [ subscriber, subscription] = pair;

        if (subscription.acceptsModel(dataModel.getID())) {
            subscriber->receiveData(dataModel);
        }
    }
}

void IDataPublisher::subscribe(
        std::shared_ptr<IDataSubscriber> subscriber, unsigned int modelID) {
    if (subscriptions.contains(subscriber)) {
        subscriptions.at(subscriber).addModel(modelID);
    } else {
        subscriptions.emplace(subscriber, Subscription(modelID));
    }
}

void IDataPublisher::unsubscribe(
        std::shared_ptr<IDataSubscriber> subscriber, unsigned int modelID) {
    if (!subscriptions.contains(subscriber)) { return; }

    if (modelID == 0) {
        subscriptions.erase(subscriber);
    } else {
        subscriptions.at(subscriber).removeModel(modelID);
    }
}

}  // namespace basil
