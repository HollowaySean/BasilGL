#pragma once

#include <errno.h>

#include <fmt/format.h>

#include <map>
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
            if (item.value().is_array()) {
                // Flatten inner array
                std::vector<T> row = vectorFromJSONArray<T>(key, item.value());
                for (T value : row) {
                    vector.push_back(value);
                }

            } else if (TypeMap<T>::isCorrectType(item.value())) {
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
    addUniform(std::shared_ptr<ShaderUniformModel> model,
            json json,
            const std::string& name) {
        const std::string_view typeKey = TypeMap<T>::key;

        if (json.is_array()) {
            // Check if uniform is in matrix format
            int vectorWidth;
            if (json.at(0).is_array()) {
                vectorWidth = json.at(0).size();
            } else {
                vectorWidth = 1;
            }

            // Array of values
            std::vector<T> vector = vectorFromJSONArray<T>(name, json);

            auto uniform = std::make_shared<GLUniformVector<T>>(
                vector, name, json.size(), vectorWidth, 1);
            model->addUniform(uniform);

            logger.log(
                fmt::format(LOG_VECTOR_ADDED,
                    typeKey, name.c_str(), json.dump()),
                LogLevel::DEBUG);

        } else if (TypeMap<T>::isCorrectType(json)) {
            // Scalar value
            T scalar = json;
            auto uniform = std::make_shared<GLUniformScalar<T>>(
                scalar, name);
            model->addUniform(uniform);

            logger.log(
                fmt::format(LOG_SCALAR_ADDED,
                    typeKey, name.c_str(), json.dump()),
                LogLevel::DEBUG);
        } else {
            // Incorrect type
            logger.log(
                fmt::format(LOG_TYPE_ERROR,
                    typeKey, name.c_str(), json.dump()),
                LogLevel::ERROR);
        }

        return model;
    }

    static std::shared_ptr<ShaderUniformModel>
    addUniforms(
        std::shared_ptr<ShaderUniformModel> model,
        json json);

    static std::shared_ptr<ShaderUniformModel>
    addTextures(
        std::shared_ptr<ShaderUniformModel> model,
        json json,
        std::filesystem::path basePath);

    static std::shared_ptr<ShaderUniformModel>
    addCubemaps(
        std::shared_ptr<ShaderUniformModel> model,
        json json,
        std::filesystem::path basePath);

    static bool verifySubfield(
        json json,
        const std::string& index,
        const std::string& baseField,
        const std::string& subField);

    static inline const std::map<std::string, GLenum>
        CUBE_FACE_TO_ENUM_MAP = {
            { "right",  GL_TEXTURE_CUBE_MAP_POSITIVE_X },
            { "left",   GL_TEXTURE_CUBE_MAP_NEGATIVE_X },
            { "top",    GL_TEXTURE_CUBE_MAP_POSITIVE_Y },
            { "bottom", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y },
            { "front",  GL_TEXTURE_CUBE_MAP_POSITIVE_Z },
            { "back",   GL_TEXTURE_CUBE_MAP_NEGATIVE_Z }
        };

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
        "Adding texture with name \"{0}\" from file {1}";

    LOGGER_FORMAT LOG_CUBEMAP_CREATED =
        "Creating cubemap with name {0}";
    LOGGER_FORMAT LOG_CUBEMAP_FACE_ADDED =
        "Adding face \"{0}\" to cubemap from file {1}";

    LOGGER_FORMAT LOG_FIELD_MISSING =
        "No field with key \"{0}\" found in file {1}";
    LOGGER_FORMAT LOG_READING_FIELD =
        "Reading {0} from JSON file";
    LOGGER_FORMAT LOG_SUBFIELD_MISSING =
        "{0} at position {1} is missing required field \"{2}\"";
};

}   // namespace basil
