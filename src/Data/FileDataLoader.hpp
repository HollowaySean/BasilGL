#ifndef SRC_DATA_FILEDATALOADER_HPP_
#define SRC_DATA_FILEDATALOADER_HPP_

#include <fmt/core.h>

#include <errno.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <Basil/Logging.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "ShaderUniformModel.hpp"

namespace basil {

class FileDataLoader {
 public:
    static std::optional<ShaderUniformModel>
    modelFromJSON(std::filesystem::path filePath);

 private:
    static inline Logger& logger = Logger::get();

    template<class T>
    struct TypeMap {
        static const std::string_view key;
        static bool isCorrectType(json json);
    };

    template<class T>
    static std::shared_ptr<ShaderUniformModel>
    addUniforms(std::shared_ptr<ShaderUniformModel> model, json json) {
        const std::string_view typeKey = TypeMap<T>::key;
        if (!json.contains(typeKey)) {
            return model;
        }

        auto uniforms = json.at(typeKey);
        for (auto& [key, value] : uniforms.items()) {
            if (value.is_array()) {
                // Array of values
                std::vector<T> vector = value.array();
                model->addUniformValue(vector, key);

                logger.log(
                    fmt::format(LOGGER_VECTOR_ADDED,
                        typeKey, key.c_str(), value.dump()),
                    LogLevel::DEBUG);

            } else if (TypeMap<T>::isCorrectType(value)) {
                // Scalar value
                T scalar = value;
                model->addUniformValue(scalar, key);

                logger.log(
                    fmt::format(LOGGER_SCALAR_ADDED,
                        typeKey, key.c_str(), value.dump()),
                    LogLevel::DEBUG);
            } else {
                // Incorrect type
                logger.log(
                    fmt::format(LOGGER_TYPE_ERROR,
                        typeKey, key.c_str(), value.dump()),
                    LogLevel::ERROR);
            }
        }

        return model;
    }

    static inline constexpr std::string_view LOGGER_FILE_MISSING =
        "File not found at path: {0}";
    static inline constexpr std::string_view LOGGER_READ_SUCCESS =
        "Successfully parsed JSON file.";
    static inline constexpr std::string_view LOGGER_READ_FAILURE =
        "Could not read file {0}\n"
        "\t See error description: ";
    static inline constexpr std::string_view LOGGER_PARSE_FAILURE =
        "Could not parse JSON at bit {1} in file {0}\n"
        "\t See error description: ";
    static inline constexpr std::string_view LOGGER_UNIFORMS_MISSING =
        "No field with key \"uniforms\" found in file {0}";
    static inline constexpr std::string_view LOGGER_VECTOR_ADDED =
        "Adding vector {0} with name \"{1}\" and value \"{2}\"";
    static inline constexpr std::string_view LOGGER_SCALAR_ADDED =
        "Adding scalar {0} with name \"{1}\" and value \"{2}\"";
    static inline constexpr std::string_view LOGGER_TYPE_ERROR =
        "Could not parse value \"{2}\" at key \"{1}\" as {0}";
};

}  // namespace basil

#endif  // SRC_DATA_FILEDATALOADER_HPP_
