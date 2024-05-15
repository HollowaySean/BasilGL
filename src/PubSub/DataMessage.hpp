#ifndef SRC_PUBSUB_DATAMESSAGE_HPP_
#define SRC_PUBSUB_DATAMESSAGE_HPP_

#include <any>
#include <optional>

namespace basil {

/** @brief Wrapper for std::any, used for PubSub message passing */
class DataMessage {
 public:
    /** @brief Initialize DataMessage from any type */
    explicit DataMessage(const std::any& data) : data(data) {}

    /** @brief Attempt to coerce DatamMessage to type T
     *  @returns Optional containing message casted to T,
     *           or nullopt if not castable. */
    template<class T>
    std::optional<T> getData() const {
        try {
            T result = std::any_cast<T>(data);
            return std::optional(result);
        } catch (std::bad_any_cast) {
            return std::nullopt;
        }
    }

 private:
    std::any data;
};

}  // namespace basil

#endif  // SRC_PUBSUB_DATAMESSAGE_HPP_
