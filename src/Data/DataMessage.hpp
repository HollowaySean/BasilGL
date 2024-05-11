#ifndef SRC_DATA_DATAMESSAGE_HPP_
#define SRC_DATA_DATAMESSAGE_HPP_

#include <any>

#include "ShaderUniformModel.hpp"
#include "UserInputModel.hpp"

namespace basil {

class DataMessage {
 public:
    explicit DataMessage(const std::any& data) : data(data) {}

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

#endif  // SRC_DATA_DATAMESSAGE_HPP_
