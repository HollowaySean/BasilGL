#ifndef SRC_DATA_DATAMESSAGE_HPP_
#define SRC_DATA_DATAMESSAGE_HPP_

#include <memory>

#include "ShaderUniformModel.hpp"
#include "UserInputModel.hpp"

namespace basil {

#ifdef TEST_BUILD
class TestDataModel {};
using DataModel = std::variant<
    ShaderUniformModel, UserInputModel, TestDataModel>;
#else
using DataModel = std::variant<
    ShaderUniformModel, UserInputModel>;
#endif

/** @brief Interface containing data which is passed in PubSub data model. */
class DataMessage {
 public:
    explicit DataMessage(const DataModel& model) : model(model) {}

    /** @returns Type ID of DataModel */
    unsigned int getID() const {
        return model.index() + 1;
    }

    DataModel model;

    template<typename T>
    static constexpr unsigned int getIDForType();
};

// TODO(sholloway): Fix this ID system
template<> constexpr unsigned int
    DataMessage::getIDForType<ShaderUniformModel>() { return 1; }
template<> constexpr unsigned int
    DataMessage::getIDForType<UserInputModel>() { return 2; }

#ifdef TEST_BUILD
template<> constexpr unsigned int
    DataMessage::getIDForType<TestDataModel>() { return 3; }
#endif

}  // namespace basil

#endif  // SRC_DATA_DATAMESSAGE_HPP_
