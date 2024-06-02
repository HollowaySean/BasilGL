#pragma once

#include <errno.h>

#include <fmt/format.h>

#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <Basil/Packages/Logging.hpp>
#include <Basil/Packages/PubSub.hpp>

#include "Data/ShaderUniformModel.hpp"

namespace basil {

/** @brief Static class which loads ShaderUniformModel from JSON file */
class FileDataLoader {
 public:
    /** @brief Generates a ShaderUniformModel object from JSON file
     *
     *  @details File reads from the "uniforms" and "textures" field.
     *  Uniforms subfields are indexed by type, then are in key value pairs
     *  with format "[uniform name]" : [value]
     *  Textures are key-value pairs with format "[uniform name]" : "[file path]"
     *  <br><br> Example:
     *  <pre>
     *  {
     *      "uniforms" : {
     *          "float" : {
     *              "uniformName1": 1.5,
     *              "uniformName2": [3.14, -0.5]
     *          },
     *          "bool" : {
     *              "uniformName3": true,
     *              "uniformName4": false
     *          }
     *      },
     *      "textures" : {
     *          "textureName1": "file-name.jpg",
     *          "textureName2": "/absolute/path/file-name.jpg"
     *      }
     *  }
     *  </pre>
     *  @param filePath Path to .json file
    */
    static std::optional<ShaderUniformModel>
    modelFromJSON(std::filesystem::path filePath);

#ifndef TEST_BUILD

 private:
#endif
    FileDataLoader() = delete;

    static inline Logger& logger = Logger::get();

    template<GLUniformType T>
    struct TypeMap {
        static const std::string_view key;
        static bool isCorrectType(json json);
    };

    template<GLUniformType T>
    static std::vector<T> vectorFromJSONArray(
            const std::string& key, json json) {
        const std::string_view typeKey = TypeMap<T>::key;
        std::vector<T> vector;

        for (const auto& item : json.items()) {
            if (TypeMap<T>::isCorrectType(item.value())) {
                vector.push_back(item.value());
            } else {
                logger.log(
                    fmt::format(LOG_VECTOR_TYPE_ERROR,
                        typeKey, key.c_str(), item.value().dump(), item.key()),
                    LogLevel::ERROR);
            }
        }

        return vector;
    }

    template<GLUniformType T>
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
                std::vector<T> vector = vectorFromJSONArray<T>(key, value);

                auto uniform = GLUniformVector<T>(vector, key, vector.size(), 1);
                model->addUniform(uniform);

                logger.log(
                    fmt::format(LOG_VECTOR_ADDED,
                        typeKey, key.c_str(), value.dump()),
                    LogLevel::DEBUG);

            } else if (TypeMap<T>::isCorrectType(value)) {
                // Scalar value
                T scalar = value;
                auto uniform = GLUniformScalar<T>(scalar, key);
                model->addUniform(uniform);

                logger.log(
                    fmt::format(LOG_SCALAR_ADDED,
                        typeKey, key.c_str(), value.dump()),
                    LogLevel::DEBUG);
            } else {
                // Incorrect type
                logger.log(
                    fmt::format(LOG_TYPE_ERROR,
                        typeKey, key.c_str(), value.dump()),
                    LogLevel::ERROR);
            }
        }

        return model;
    }

    static std::shared_ptr<ShaderUniformModel>
    addTexture(
        std::shared_ptr<ShaderUniformModel> model,
        json json,
        std::filesystem::path basePath);

    LOGGER_FORMAT LOG_FILE_MISSING =
        "File not found at path: {0}";
    LOGGER_FORMAT LOG_READ_SUCCESS =
        "Successfully parsed JSON file.";
    LOGGER_FORMAT LOG_READ_FAILURE =
        "Could not read file {0}\n"
        "\t See error description: ";
    LOGGER_FORMAT LOG_PARSE_FAILURE =
        "Could not parse JSON at bit {1} in file {0}\n"
        "\t See error description: ";
    LOGGER_FORMAT LOG_JSON_EMPTY =
        "Empty JSON found in file {0}";
    LOGGER_FORMAT LOG_UNIFORMS_MISSING =
        "No field with key \"uniforms\" found in file {0}";
    LOGGER_FORMAT LOG_VECTOR_ADDED =
        "Adding vector {0} with name \"{1}\" and value \"{2}\"";
    LOGGER_FORMAT LOG_SCALAR_ADDED =
        "Adding scalar {0} with name \"{1}\" and value \"{2}\"";
    LOGGER_FORMAT LOG_TYPE_ERROR =
        "Could not parse value \"{2}\" at key \"{1}\" as {0}";
    LOGGER_FORMAT LOG_VECTOR_TYPE_ERROR =
        "Could not coerce value \"{2}\" from key \"{1}\" "
        "at position {3} to type {0}";
    LOGGER_FORMAT LOG_TEXTURE_ADDED =
        "Adding texture with name {0} from file {1}";
    LOGGER_FORMAT LOG_TEXTURES_MISSING =
        "No field with key \"textures\" found in file {0}";
};

}   // namespace basil
